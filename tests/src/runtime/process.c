#include "capture_internal.h"
#include "process_internal.h"
#include "time_internal.h"
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void	process_result_init(t_process_result *result)
{
	memset(result, 0, sizeof(*result));
	result->exit_code = -1;
}

void	process_result_destroy(t_process_result *result)
{
	free(result->stdout_data);
	free(result->stderr_data);
	memset(result, 0, sizeof(*result));
}

int	process_set_nonblocking(int fd)
{
	int	flags;

	flags = fcntl(fd, F_GETFL);
	if (flags < 0)
		return (-1);
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
		return (-1);
	return (0);
}

int	process_write_all(int fd, const void *data, size_t length)
{
	const unsigned char	*bytes;
	ssize_t			count;

	bytes = data;
	while (length > 0)
	{
		count = write(fd, bytes, length);
		if (count > 0)
		{
			bytes += count;
			length -= (size_t)count;
			continue ;
		}
		if (count < 0 && errno == EINTR)
			continue ;
		return (-1);
	}
	return (0);
}

int	process_read_all(int fd, void *data, size_t length, bool *clean_eof)
{
	unsigned char	*bytes;
	ssize_t		count;

	bytes = data;
	*clean_eof = false;
	while (length > 0)
	{
		count = read(fd, bytes, length);
		if (count > 0)
		{
			bytes += count;
			length -= (size_t)count;
			continue ;
		}
		if (count == 0)
		{
			*clean_eof = true;
			return (-1);
		}
		if (errno == EINTR)
			continue ;
		return (-1);
	}
	return (0);
}

static void	close_pipe(int pipe_fds[2])
{
	if (pipe_fds[0] >= 0)
		close(pipe_fds[0]);
	if (pipe_fds[1] >= 0)
		close(pipe_fds[1]);
	pipe_fds[0] = -1;
	pipe_fds[1] = -1;
}

static void	terminate_process_group(pid_t child)
{
	if (kill(-child, SIGKILL) != 0 && errno == ESRCH)
		(void)kill(child, SIGKILL);
}

static int	drain_capture(int *fd, t_capture_buffer *buffer)
{
	bool	eof;

	if (*fd < 0)
		return (0);
	if (capture_buffer_read_fd(buffer, *fd, &eof) != 0)
		return (-1);
	if (eof)
	{
		close(*fd);
		*fd = -1;
	}
	return (0);
}

static int	poll_capture_fds(int stdout_fd, int stderr_fd, int timeout_ms)
{
	struct pollfd	fds[2];
	nfds_t			count;

	count = 0;
	if (stdout_fd >= 0)
	{
		fds[count].fd = stdout_fd;
		fds[count].events = POLLIN | POLLHUP;
		fds[count].revents = 0;
		count++;
	}
	if (stderr_fd >= 0)
	{
		fds[count].fd = stderr_fd;
		fds[count].events = POLLIN | POLLHUP;
		fds[count].revents = 0;
		count++;
	}
	if (poll(fds, count, timeout_ms) < 0 && errno != EINTR)
		return (-1);
	return (0);
}

static int	collect_child(pid_t child, int stdout_fd, int stderr_fd,
		uint32_t timeout_ms, size_t limit, t_process_result *result)
{
	t_capture_buffer	stdout_buffer;
	t_capture_buffer	stderr_buffer;
	uint64_t		start;
	uint64_t		deadline;
	int				wait_status;
	pid_t			waited;
	bool			child_done;
	int				failure;

	capture_buffer_init(&stdout_buffer, limit);
	capture_buffer_init(&stderr_buffer, limit);
	start = time_now_ns();
	deadline = start + (uint64_t)timeout_ms * 1000000ULL;
	child_done = false;
	failure = 0;
	wait_status = 0;
	while (!child_done)
	{
		if (drain_capture(&stdout_fd, &stdout_buffer) != 0
			|| drain_capture(&stderr_fd, &stderr_buffer) != 0)
		{
			failure = -1;
			break ;
		}
		waited = waitpid(child, &wait_status, WNOHANG);
		if (waited == child)
			child_done = true;
		else if (waited < 0 && errno != EINTR)
		{
			failure = -1;
			break ;
		}
		if (!child_done && time_deadline_expired(deadline))
		{
			result->timed_out = true;
			terminate_process_group(child);
			while (waitpid(child, &wait_status, 0) < 0 && errno == EINTR)
				;
			child_done = true;
		}
		if (!child_done && poll_capture_fds(stdout_fd, stderr_fd,
				time_poll_timeout_ms(deadline, 50)) != 0)
		{
			failure = -1;
			break ;
		}
	}
	if (failure != 0 && !child_done)
	{
		terminate_process_group(child);
		while (waitpid(child, &wait_status, 0) < 0 && errno == EINTR)
			;
	}
	if (stdout_fd >= 0)
		(void)drain_capture(&stdout_fd, &stdout_buffer);
	if (stderr_fd >= 0)
		(void)drain_capture(&stderr_fd, &stderr_buffer);
	if (stdout_fd >= 0)
		close(stdout_fd);
	if (stderr_fd >= 0)
		close(stderr_fd);
	result->duration_ns = time_now_ns() - start;
	if (WIFEXITED(wait_status))
	{
		result->exited = true;
		result->exit_code = WEXITSTATUS(wait_status);
	}
	else if (WIFSIGNALED(wait_status))
	{
		result->signaled = true;
		result->signal_number = WTERMSIG(wait_status);
	}
	result->stdout_length = stdout_buffer.length;
	result->stderr_length = stderr_buffer.length;
	result->stdout_truncated = stdout_buffer.truncated;
	result->stderr_truncated = stderr_buffer.truncated;
	result->stdout_data = capture_buffer_take(&stdout_buffer);
	result->stderr_data = capture_buffer_take(&stderr_buffer);
	capture_buffer_destroy(&stdout_buffer);
	capture_buffer_destroy(&stderr_buffer);
	if (result->stdout_data == NULL || result->stderr_data == NULL)
		failure = -1;
	return (failure);
}

static void	exec_child(const char *const argv[], int stdout_pipe[2],
		int stderr_pipe[2])
{
	char	message[512];
	int	length;

	(void)setpgid(0, 0);
	close(stdout_pipe[0]);
	close(stderr_pipe[0]);
	if (dup2(stdout_pipe[1], STDOUT_FILENO) < 0
		|| dup2(stderr_pipe[1], STDERR_FILENO) < 0)
		_exit(126);
	close(stdout_pipe[1]);
	close(stderr_pipe[1]);
	execvp(argv[0], (char *const *)argv);
	length = snprintf(message, sizeof(message), "42tester: exec %s: %s\n",
			argv[0], strerror(errno));
	if (length > 0)
		(void)write(STDERR_FILENO, message, (size_t)length);
	_exit(127);
}

int	process_run_capture(const char *const argv[], uint32_t timeout_ms,
		size_t capture_limit, t_process_result *result)
{
	int		stdout_pipe[2];
	int		stderr_pipe[2];
	pid_t	child;

	stdout_pipe[0] = -1;
	stdout_pipe[1] = -1;
	stderr_pipe[0] = -1;
	stderr_pipe[1] = -1;
	process_result_init(result);
	if (pipe(stdout_pipe) != 0 || pipe(stderr_pipe) != 0)
	{
		close_pipe(stdout_pipe);
		close_pipe(stderr_pipe);
		return (-1);
	}
	child = fork();
	if (child < 0)
	{
		close_pipe(stdout_pipe);
		close_pipe(stderr_pipe);
		return (-1);
	}
	if (child == 0)
		exec_child(argv, stdout_pipe, stderr_pipe);
	close(stdout_pipe[1]);
	stdout_pipe[1] = -1;
	close(stderr_pipe[1]);
	stderr_pipe[1] = -1;
	(void)setpgid(child, child);
	if (process_set_nonblocking(stdout_pipe[0]) != 0
		|| process_set_nonblocking(stderr_pipe[0]) != 0)
	{
		terminate_process_group(child);
		while (waitpid(child, NULL, 0) < 0 && errno == EINTR)
			;
		close_pipe(stdout_pipe);
		close_pipe(stderr_pipe);
		return (-1);
	}
	return (collect_child(child, stdout_pipe[0], stderr_pipe[0], timeout_ms,
			capture_limit, result));
}

