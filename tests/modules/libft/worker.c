#include "libft_module.h"
#include "test_case.h"

int	libft_test_strlen_basic(t_test_context *context);
int	libft_test_strlen_empty(t_test_context *context);
int	libft_test_strlen_boundary(t_test_context *context);
int	libft_test_strlen_random(t_test_context *context);

int	libft_register_tests(t_test_registry *registry)
{
	static const t_test_case	tests[] = {
	{
		.id = "ft_strlen.basic",
		.group = "ft_strlen",
		.type = TEST_BASIC,
		.timeout_ms = 1000,
		.default_iterations = 1,
		.run = libft_test_strlen_basic
	},
	{
		.id = "ft_strlen.empty",
		.group = "ft_strlen",
		.type = TEST_EDGE,
		.timeout_ms = 1000,
		.default_iterations = 1,
		.run = libft_test_strlen_empty
	},
	{
		.id = "ft_strlen.boundary",
		.group = "ft_strlen",
		.type = TEST_BOUNDARY,
		.timeout_ms = 1000,
		.default_iterations = 1,
		.run = libft_test_strlen_boundary
	},
	{
		.id = "ft_strlen.random",
		.group = "ft_strlen",
		.type = TEST_RANDOM,
		.timeout_ms = 1000,
		.default_iterations = 100,
		.run = libft_test_strlen_random
	}
	};
	size_t	index;

	index = 0;
	while (index < sizeof(tests) / sizeof(tests[0]))
	{
		if (registry_add(registry, &tests[index]) != 0)
			return (-1);
		index++;
	}
	return (0);
}

