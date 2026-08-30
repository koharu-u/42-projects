#include "libft_module.h"
#include "test_case.h"

int	libft_register_ft_strlen_tests(t_test_registry *registry);
int	libft_register_ft_isalpha_tests(t_test_registry *registry);
int	libft_register_ft_isdigit_tests(t_test_registry *registry);
int	libft_register_ft_memset_tests(t_test_registry *registry);
int	libft_register_ft_memcpy_tests(t_test_registry *registry);

int	libft_register_tests(t_test_registry *registry)
{
	(void)registry;
# ifdef LIBFT_INCLUDE_FT_STRLEN
	if (libft_register_ft_strlen_tests(registry) != 0)
		return (-1);
# endif
# ifdef LIBFT_INCLUDE_FT_ISALPHA
	if (libft_register_ft_isalpha_tests(registry) != 0)
		return (-1);
# endif
# ifdef LIBFT_INCLUDE_FT_ISDIGIT
	if (libft_register_ft_isdigit_tests(registry) != 0)
		return (-1);
# endif
# ifdef LIBFT_INCLUDE_FT_MEMSET
	if (libft_register_ft_memset_tests(registry) != 0)
		return (-1);
# endif
# ifdef LIBFT_INCLUDE_FT_MEMCPY
	if (libft_register_ft_memcpy_tests(registry) != 0)
		return (-1);
# endif
	return (0);
}
