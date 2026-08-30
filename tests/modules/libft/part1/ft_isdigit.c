#include "libft_api.h"
#include "registry.h"
#include "test_case.h"

static int	libft_test_isdigit_basic(t_test_context *context)
{
	int	character;
	int	expected;
	int	actual;

	character = -1;
	while (character <= 256)
	{
		expected = (character >= '0' && character <= '9');
		actual = (ft_isdigit(character) != 0);
		if (actual != expected)
			return (test_context_fail(context,
					"character=%d expected=%d actual=%d", character, expected,
					actual));
		character++;
	}
	return (TEST_CALLBACK_PASS);
}

int	libft_register_ft_isdigit_tests(t_test_registry *registry)
{
	static const t_test_case	tests[] = {
	{"ft_isdigit.basic", "ft_isdigit", TEST_BASIC, 1000, 1,
		libft_test_isdigit_basic}
	};

	if (registry_add(registry, &tests[0]) != 0)
		return (-1);
	return (0);
}
