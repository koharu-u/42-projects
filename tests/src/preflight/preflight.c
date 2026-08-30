#include "preflight.h"
#include <stdlib.h>
#include <string.h>

void	check_result_init(t_check_result *result, const char *name)
{
	memset(result, 0, sizeof(*result));
	result->name = strdup(name);
	result->status = CHECK_UNKNOWN;
	result->exit_code = -1;
}

void	check_result_destroy(t_check_result *result)
{
	free(result->name);
	free(result->reason);
	free(result->stdout_data);
	free(result->stderr_data);
	memset(result, 0, sizeof(*result));
}

int	check_result_set_reason(t_check_result *result, const char *reason)
{
	char	*copy;

	copy = strdup(reason);
	if (copy == NULL)
		return (-1);
	free(result->reason);
	result->reason = copy;
	return (0);
}

const char	*check_status_name(t_check_status status)
{
	if (status == CHECK_PASS)
		return ("PASS");
	if (status == CHECK_FAIL)
		return ("FAIL");
	if (status == CHECK_SKIP)
		return ("SKIP");
	if (status == CHECK_MISSING)
		return ("MISSING");
	if (status == CHECK_BLOCKED)
		return ("BLOCKED");
	return ("UNKNOWN");
}
