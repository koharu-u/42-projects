#ifndef PROCESS_INTERNAL_H
# define PROCESS_INTERNAL_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>

typedef struct s_process_result
{
	bool			exited;
	bool			signaled;
	bool			timed_out;
	int			exit_code;
	int			signal_number;
	uint64_t	duration_ns;
	unsigned char	*stdout_data;
	size_t		stdout_length;
	unsigned char	*stderr_data;
	size_t		stderr_length;
	bool			stdout_truncated;
	bool			stderr_truncated;
}   t_process_result;

void	process_result_init(t_process_result *result);
void	process_result_destroy(t_process_result *result);
int	process_run_capture(const char *const argv[], uint32_t timeout_ms,
		size_t capture_limit, t_process_result *result);
int	process_set_nonblocking(int fd);
int	process_write_all(int fd, const void *data, size_t length);
int	process_read_all(int fd, void *data, size_t length, bool *clean_eof);
char	*path_find_executable(const char *name);

#endif

