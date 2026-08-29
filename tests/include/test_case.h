#ifndef TEST_CASE_H
# define TEST_CASE_H

# include "rng.h"
# include "tester.h"
# include <stddef.h>
# include <stdint.h>

typedef enum e_test_type
{
	TEST_BASIC,
	TEST_BOUNDARY,
	TEST_EDGE,
	TEST_DIFFERENTIAL,
	TEST_RANDOM,
	TEST_STRESS,
	TEST_ALLOCATION,
	TEST_MEMORY
}   t_test_type;

typedef struct s_test_context
{
	uint64_t	global_seed;
	uint64_t	test_seed;
	uint64_t	iteration;
	t_rng		rng;
	int		_control_fd;
	char		diagnostic[TESTER_DIAGNOSTIC_CAP];
	size_t		diagnostic_length;
}   t_test_context;

typedef int	(*t_test_fn)(t_test_context *context);

typedef struct s_test_case
{
	const char		*id;
	const char		*group;
	t_test_type		type;
	uint32_t		timeout_ms;
	uint32_t		default_iterations;
	t_test_fn		run;
}   t_test_case;

typedef struct s_test_invocation
{
	const t_test_case	*test;
	uint64_t			global_seed;
	uint64_t			test_seed;
	uint64_t			iteration;
	uint32_t			timeout_ms;
}   t_test_invocation;

enum e_test_callback_result
{
	TEST_CALLBACK_PASS = 0,
	TEST_CALLBACK_FAIL = 1,
	TEST_CALLBACK_SKIP = 77
};

int	test_context_fail(t_test_context *context, const char *format, ...);
int	test_context_describe_input(t_test_context *context,
		const char *format, ...);

#endif
