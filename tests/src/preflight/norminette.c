#include "preflight.h"
#include "process_internal.h"
#include "tester.h"
#include <stdlib.h>

static void	check_take_process_result(t_check_result *check,
		t_process_result *process)
{
	check->exit_code = process->exit_code;
	check->signal_number = process->signal_number;
	check->timed_out = process->timed_out;
	check->duration_ns = process->duration_ns;
	check->stdout_data = process->stdout_data;
	check->stdout_length = process->stdout_length;
	check->stdout_truncated = process->stdout_truncated;
	check->stderr_data = process->stderr_data;
	check->stderr_length = process->stderr_length;
	check->stderr_truncated = process->stderr_truncated;
	process->stdout_data = NULL;
	process->stderr_data = NULL;
}

int	preflight_run_norminette(const char *target_path, t_check_result *result)
{
	char			*executable;
	const char		*arguments[3];
	t_process_result	process;
	int				status;

	check_result_init(result, "Norminette");
	if (result->name == NULL)
		return (-1);
	executable = path_find_executable("norminette");
	if (executable == NULL)
	{
		result->status = CHECK_UNKNOWN;
		return (check_result_set_reason(result,
				"norminette executable not found"));
	}
	arguments[0] = executable;
	arguments[1] = target_path;
	arguments[2] = NULL;
	process_result_init(&process);
	status = process_run_capture(arguments, TESTER_TOOL_TIMEOUT_MS,
			TESTER_CAPTURE_LIMIT, &process);
	free(executable);
	if (status != 0)
	{
		process_result_destroy(&process);
		result->status = CHECK_UNKNOWN;
		if (check_result_set_reason(result,
				"failed to execute norminette") != 0)
			return (-1);
		return (0);
	}
	check_take_process_result(result, &process);
	if (process.timed_out)
	{
		result->status = CHECK_UNKNOWN;
		status = check_result_set_reason(result, "norminette timed out");
	}
	else if (process.signaled)
	{
		result->status = CHECK_UNKNOWN;
		status = check_result_set_reason(result,
				"norminette terminated by a signal");
	}
	else if (!process.exited || process.exit_code == 126
		|| process.exit_code == 127)
	{
		result->status = CHECK_UNKNOWN;
		status = check_result_set_reason(result,
				"norminette could not be executed");
	}
	else if (process.exit_code == 0)
	{
		result->status = CHECK_PASS;
		status = 0;
	}
	else
	{
		result->status = CHECK_FAIL;
		status = 0;
	}
	process_result_destroy(&process);
	return (status);
}

