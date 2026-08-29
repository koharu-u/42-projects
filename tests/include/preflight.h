#ifndef PREFLIGHT_H
# define PREFLIGHT_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>

typedef enum e_check_status
{
	CHECK_PASS,
	CHECK_FAIL,
	CHECK_UNKNOWN,
	CHECK_SKIP
}   t_check_status;

typedef struct s_check_result
{
	char			*name;
	t_check_status	status;
	char			*reason;
	int			exit_code;
	int			signal_number;
	bool			timed_out;
	uint64_t	duration_ns;
	unsigned char	*stdout_data;
	size_t		stdout_length;
	unsigned char	*stderr_data;
	size_t		stderr_length;
	bool			stdout_truncated;
	bool			stderr_truncated;
}   t_check_result;

void		check_result_init(t_check_result *result, const char *name);
void		check_result_destroy(t_check_result *result);
int		check_result_set_reason(t_check_result *result, const char *reason);
const char	*check_status_name(t_check_status status);
int		preflight_run_norminette(const char *target_path,
				t_check_result *result);

#endif

