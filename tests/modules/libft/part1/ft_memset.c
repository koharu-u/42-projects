#include "libft_api.h"
#include "registry.h"
#include "test_case.h"
#include <string.h>

static int	libft_test_memset_basic(t_test_context *context)
{
	unsigned char	actual[16];
	unsigned char	expected[16];
	void			*returned;

	memset(actual, 0x11, sizeof(actual));
	memset(expected, 0x11, sizeof(expected));
	returned = ft_memset(actual + 3, 0x1ff, 8);
	memset(expected + 3, 0x1ff, 8);
	if (returned != actual + 3 || memcmp(actual, expected, sizeof(actual)) != 0)
		return (test_context_fail(context,
				"return value or bytes differ for offset=3 length=8 value=511"));
	return (TEST_CALLBACK_PASS);
}

int	libft_register_ft_memset_tests(t_test_registry *registry)
{
	static const t_test_case	tests[] = {
	{"ft_memset.basic", "ft_memset", TEST_BASIC, 1000, 1,
		libft_test_memset_basic}
	};

	if (registry_add(registry, &tests[0]) != 0)
		return (-1);
	return (0);
}
