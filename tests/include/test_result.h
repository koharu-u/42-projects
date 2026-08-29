#ifndef TEST_RESULT_H
# define TEST_RESULT_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>

typedef enum e_test_status
{
	TEST_PASS,
	TEST_FAIL,
	TEST_CRASH,
	TEST_TIMEOUT,
	TEST_LEAK,
	TEST_SKIP,
	TEST_ERROR
}   t_test_status;

typedef struct s_test_result
{
	char			*test_id;
	t_test_status	status;
	uint64_t	global_seed;
	uint64_t	test_seed;
	uint64_t	iteration;
	uint64_t	duration_ns;
	int			exit_code;
	int			signal_number;
	char			*diagnostic;
	char			*input_description;
	unsigned char	*stdout_data;
	size_t		stdout_length;
	unsigned char	*stderr_data;
	size_t		stderr_length;
	bool			stdout_truncated;
	bool			stderr_truncated;
}   t_test_result;

void		test_result_init(t_test_result *result);
void		test_result_destroy(t_test_result *result);
int		test_result_set_id(t_test_result *result, const char *test_id);
const char	*test_status_name(t_test_status status);
const char	*test_signal_name(int signal_number);

#endif
