#include "libft_api.h"
#include "registry.h"
#include "test_case.h"
#include <string.h>

static int	libft_test_memcpy_basic(t_test_context *context)
{
	const unsigned char	source[] = {0, 1, 2, 3, 4, 5, 255};
	unsigned char		actual[sizeof(source) + 2];
	unsigned char		expected[sizeof(source) + 2];
	void				*returned;

	memset(actual, 0xaa, sizeof(actual));
	memset(expected, 0xaa, sizeof(expected));
	returned = ft_memcpy(actual + 1, source, sizeof(source));
	memcpy(expected + 1, source, sizeof(source));
	if (returned != actual + 1 || memcmp(actual, expected, sizeof(actual)) != 0)
		return (test_context_fail(context,
				"return value or bytes differ for non-overlapping input"));
	return (TEST_CALLBACK_PASS);
}

int	libft_register_ft_memcpy_tests(t_test_registry *registry)
{
	static const t_test_case	tests[] = {
	{"ft_memcpy.basic", "ft_memcpy", TEST_BASIC, 1000, 1,
		libft_test_memcpy_basic}
	};

	if (registry_add(registry, &tests[0]) != 0)
		return (-1);
	return (0);
}
