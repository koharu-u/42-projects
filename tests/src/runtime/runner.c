#include "capture_internal.h"
#include "process_internal.h"
#include "runner.h"
#include "time_internal.h"
#include "wire_internal.h"
#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static void	runner_close_pipe(int pipe_fds[2])
{
	if (pipe_fds[0] >= 0)
		close(pipe_fds[0]);
	if (pipe_fds[1] >= 0)
		close(pipe_fds[1]);
	pipe_fds[0] = -1;
	pipe_fds[1] = -1;
}

static void	runner_kill_group(pid_t child)
{
	if (kill(-child, SIGKILL) != 0 && errno == ESRCH)
		(void)kill(child, SIGKILL);
}

static t_test_status	callback_status(int callback_result)
{
	if (callback_result == TEST_CALLBACK_PASS)
		return (TEST_PASS);
	if (callback_result == TEST_CALLBACK_FAIL)
		return (TEST_FAIL);
	if (callback_result == TEST_CALLBACK_SKIP)
		return (TEST_SKIP);
	return (TEST_ERROR);
}

static void	runner_child(const t_test_invocation *invocation,
		int control_pipe[2], int stdout_pipe[2], int stderr_pipe[2])
{
	t_test_context	context;
	t_test_status	status;
	int				callback_result;

	(void)setpgid(0, 0);
	close(control_pipe[0]);
	close(stdout_pipe[0]);
	close(stderr_pipe[0]);
	if (dup2(stdout_pipe[1], STDOUT_FILENO) < 0
		|| dup2(stderr_pipe[1], STDERR_FILENO) < 0)
		_exit(126);
	close(stdout_pipe[1]);
	close(stderr_pipe[1]);
	(void)setvbuf(stdout, NULL, _IONBF, 0);
	(void)setvbuf(stderr, NULL, _IONBF, 0);
	memset(&context, 0, sizeof(context));
	context.global_seed = invocation->global_seed;
	context.test_seed = invocation->test_seed;
	context.iteration = invocation->iteration;
	context._control_fd = control_pipe[1];
	rng_init(&context.rng, context.test_seed);
	callback_result = invocation->test->run(&context);
	status = callback_status(callback_result);
	if (status == TEST_FAIL && context.diagnostic_length == 0)
		(void)test_context_fail(&context, "test callback reported failure");
	if (status == TEST_ERROR && context.diagnostic_length == 0)
		(void)snprintf(context.diagnostic, sizeof(context.diagnostic),
			"test callback returned invalid status %d", callback_result);
	(void)child_wire_send(control_pipe[1], status, context.diagnostic);
	close(control_pipe[1]);
	if (status == TEST_PASS || status == TEST_SKIP)
		_exit(0);
	_exit(1);
}

static int	runner_drain_fd(int *fd, t_capture_buffer *buffer)
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

static int	runner_poll(int stdout_fd, int stderr_fd, int timeout_ms)
{
	struct pollfd	fds[2];
	nfds_t			count;

	count = 0;
	if (stdout_fd >= 0)
	{
		fds[count].fd = stdout_fd;
		fds[count].events = POLLIN | POLLHUP;
		fds[count++].revents = 0;
	}
	if (stderr_fd >= 0)
	{
		fds[count].fd = stderr_fd;
		fds[count].events = POLLIN | POLLHUP;
		fds[count++].revents = 0;
	}
	if (poll(fds, count, timeout_ms) < 0 && errno != EINTR)
		return (-1);
	return (0);
}

static int	runner_watch(pid_t child, int *stdout_fd, int *stderr_fd,
		uint32_t timeout_ms, t_capture_buffer *stdout_buffer,
		t_capture_buffer *stderr_buffer, int *wait_status, bool *timed_out)
{
	uint64_t	deadline;
	pid_t		waited;

	deadline = time_now_ns() + (uint64_t)timeout_ms * 1000000ULL;
	while (true)
	{
		if (runner_drain_fd(stdout_fd, stdout_buffer) != 0
			|| runner_drain_fd(stderr_fd, stderr_buffer) != 0)
			return (-1);
		waited = waitpid(child, wait_status, WNOHANG);
		if (waited == child)
			return (0);
		if (waited < 0 && errno != EINTR)
			return (-1);
		if (time_deadline_expired(deadline))
		{
			*timed_out = true;
			runner_kill_group(child);
			while (waitpid(child, wait_status, 0) < 0 && errno == EINTR)
				;
			return (0);
		}
		if (runner_poll(*stdout_fd, *stderr_fd,
				time_poll_timeout_ms(deadline, 25)) != 0)
			return (-1);
	}
}

static int	runner_prepare_result(const t_test_invocation *invocation,
		t_test_result *result)
{
	test_result_init(result);
	result->global_seed = invocation->global_seed;
	result->test_seed = invocation->test_seed;
	result->iteration = invocation->iteration;
	return (test_result_set_id(result, invocation->test->id));
}

static void	runner_take_capture(t_test_result *result,
		t_capture_buffer *stdout_buffer, t_capture_buffer *stderr_buffer)
{
	result->stdout_length = stdout_buffer->length;
	result->stderr_length = stderr_buffer->length;
	result->stdout_truncated = stdout_buffer->truncated;
	result->stderr_truncated = stderr_buffer->truncated;
	result->stdout_data = capture_buffer_take(stdout_buffer);
	result->stderr_data = capture_buffer_take(stderr_buffer);
}

static void	runner_classify(t_test_result *result, int wait_status,
		bool timed_out, int control_fd)
{
	t_test_status	wire_status;
	char			*wire_diagnostic;
	char			*input_description;
	int				wire_status_code;

	result->exit_code = -1;
	if (WIFEXITED(wait_status))
		result->exit_code = WEXITSTATUS(wait_status);
	wire_diagnostic = NULL;
	input_description = NULL;
	wire_status_code = child_wire_receive(control_fd, &wire_status,
			&wire_diagnostic, &input_description);
	result->input_description = input_description;
	if (result->input_description == NULL)
		result->input_description = strdup("");
	if (timed_out)
	{
		free(wire_diagnostic);
		result->status = TEST_TIMEOUT;
		result->signal_number = SIGKILL;
		result->diagnostic = strdup("test exceeded its timeout");
		return ;
	}
	if (WIFSIGNALED(wait_status))
	{
		free(wire_diagnostic);
		result->status = TEST_CRASH;
		result->signal_number = WTERMSIG(wait_status);
		result->diagnostic = strdup(test_signal_name(result->signal_number));
		return ;
	}
	if (!WIFEXITED(wait_status)
		|| wire_status_code != 0)
	{
		free(wire_diagnostic);
		result->status = TEST_ERROR;
		result->diagnostic = strdup("child returned no valid result");
		return ;
	}
	result->status = wire_status;
	result->diagnostic = wire_diagnostic;
}

int	runner_execute(const t_test_invocation *invocation,
		const t_sandbox_options *sandbox, t_test_result *result)
{
	int			control_pipe[2];
	int			stdout_pipe[2];
	int			stderr_pipe[2];
	pid_t		child;
	t_capture_buffer	stdout_buffer;
	t_capture_buffer	stderr_buffer;
	uint64_t		start;
	int				wait_status;
	bool			timed_out;
	int				watch_status;

	if (runner_prepare_result(invocation, result) != 0)
		return (-1);
	control_pipe[0] = -1;
	control_pipe[1] = -1;
	stdout_pipe[0] = -1;
	stdout_pipe[1] = -1;
	stderr_pipe[0] = -1;
	stderr_pipe[1] = -1;
	if (pipe(control_pipe) != 0 || pipe(stdout_pipe) != 0
		|| pipe(stderr_pipe) != 0)
		return (runner_close_pipe(control_pipe), runner_close_pipe(stdout_pipe),
			runner_close_pipe(stderr_pipe), -1);
	(void)fflush(NULL);
	start = time_now_ns();
	child = fork();
	if (child < 0)
		return (runner_close_pipe(control_pipe), runner_close_pipe(stdout_pipe),
			runner_close_pipe(stderr_pipe), -1);
	if (child == 0)
		runner_child(invocation, control_pipe, stdout_pipe, stderr_pipe);
	close(control_pipe[1]);
	control_pipe[1] = -1;
	close(stdout_pipe[1]);
	stdout_pipe[1] = -1;
	close(stderr_pipe[1]);
	stderr_pipe[1] = -1;
	(void)setpgid(child, child);
	capture_buffer_init(&stdout_buffer, sandbox->capture_limit);
	capture_buffer_init(&stderr_buffer, sandbox->capture_limit);
	timed_out = false;
	wait_status = 0;
	watch_status = -1;
	if (process_set_nonblocking(stdout_pipe[0]) == 0
		&& process_set_nonblocking(stderr_pipe[0]) == 0)
		watch_status = runner_watch(child, &stdout_pipe[0], &stderr_pipe[0],
				invocation->timeout_ms, &stdout_buffer, &stderr_buffer,
				&wait_status, &timed_out);
	if (watch_status != 0)
	{
		runner_kill_group(child);
		while (waitpid(child, &wait_status, 0) < 0 && errno == EINTR)
			;
	}
	else
		runner_kill_group(child);
	(void)runner_drain_fd(&stdout_pipe[0], &stdout_buffer);
	(void)runner_drain_fd(&stderr_pipe[0], &stderr_buffer);
	if (stdout_pipe[0] >= 0)
		close(stdout_pipe[0]);
	if (stderr_pipe[0] >= 0)
		close(stderr_pipe[0]);
	result->duration_ns = time_now_ns() - start;
	runner_take_capture(result, &stdout_buffer, &stderr_buffer);
	capture_buffer_destroy(&stdout_buffer);
	capture_buffer_destroy(&stderr_buffer);
	if (watch_status != 0)
	{
		result->status = TEST_ERROR;
		result->diagnostic = strdup("runner failed while monitoring child");
		result->input_description = strdup("");
	}
	else
		runner_classify(result, wait_status, timed_out, control_pipe[0]);
	close(control_pipe[0]);
	if (result->stdout_data == NULL || result->stderr_data == NULL
		|| result->diagnostic == NULL || result->input_description == NULL)
		return (-1);
	return (0);
}
