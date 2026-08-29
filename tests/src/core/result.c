#include "test_case.h"
#include "test_result.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	test_result_init(t_test_result *result)
{
	memset(result, 0, sizeof(*result));
	result->status = TEST_ERROR;
	result->exit_code = -1;
}

void	test_result_destroy(t_test_result *result)
{
	free(result->test_id);
	free(result->diagnostic);
	free(result->input_description);
	free(result->stdout_data);
	free(result->stderr_data);
	memset(result, 0, sizeof(*result));
}

int	test_result_set_id(t_test_result *result, const char *test_id)
{
	char	*copy;

	copy = strdup(test_id);
	if (copy == NULL)
		return (-1);
	free(result->test_id);
	result->test_id = copy;
	return (0);
}

const char	*test_status_name(t_test_status status)
{
	if (status == TEST_PASS)
		return ("PASS");
	if (status == TEST_FAIL)
		return ("FAIL");
	if (status == TEST_CRASH)
		return ("CRASH");
	if (status == TEST_TIMEOUT)
		return ("TIMEOUT");
	if (status == TEST_LEAK)
		return ("LEAK");
	if (status == TEST_SKIP)
		return ("SKIP");
	return ("ERROR");
}

int	test_context_fail(t_test_context *context, const char *format, ...)
{
	va_list	arguments;
	int		written;

	va_start(arguments, format);
	written = vsnprintf(context->diagnostic, sizeof(context->diagnostic),
			format, arguments);
	va_end(arguments);
	if (written < 0)
	{
		context->diagnostic[0] = '\0';
		context->diagnostic_length = 0;
	}
	else if ((size_t)written >= sizeof(context->diagnostic))
		context->diagnostic_length = sizeof(context->diagnostic) - 1;
	else
		context->diagnostic_length = (size_t)written;
	return (TEST_CALLBACK_FAIL);
}
