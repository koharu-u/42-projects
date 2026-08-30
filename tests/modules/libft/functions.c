#include "libft_module.h"
#include <string.h>

static const char *const	g_no_dependencies[] = {NULL};
static const char *const	g_strlen_dependencies[] = {"ft_strlen", NULL};
static const char *const	g_bzero_dependencies[] = {"ft_bzero", NULL};
static const char *const	g_putendl_dependencies[] = {
	"ft_putstr_fd", "ft_putchar_fd", NULL
};

static const t_libft_function	g_functions[] = {
	{"ft_isalpha", "ft_isalpha.c", g_no_dependencies,
		"modules/libft/part1/ft_isalpha.c", "LIBFT_INCLUDE_FT_ISALPHA=1"},
	{"ft_isdigit", "ft_isdigit.c", g_no_dependencies,
		"modules/libft/part1/ft_isdigit.c", "LIBFT_INCLUDE_FT_ISDIGIT=1"},
	{"ft_isalnum", "ft_isalnum.c", g_no_dependencies, NULL, NULL},
	{"ft_isascii", "ft_isascii.c", g_no_dependencies, NULL, NULL},
	{"ft_isprint", "ft_isprint.c", g_no_dependencies, NULL, NULL},
	{"ft_strlen", "ft_strlen.c", g_no_dependencies,
		"modules/libft/part1/ft_strlen.c", "LIBFT_INCLUDE_FT_STRLEN=1"},
	{"ft_memset", "ft_memset.c", g_no_dependencies,
		"modules/libft/part1/ft_memset.c", "LIBFT_INCLUDE_FT_MEMSET=1"},
	{"ft_bzero", "ft_bzero.c", g_no_dependencies, NULL, NULL},
	{"ft_memcpy", "ft_memcpy.c", g_no_dependencies,
		"modules/libft/part1/ft_memcpy.c", "LIBFT_INCLUDE_FT_MEMCPY=1"},
	{"ft_memmove", "ft_memmove.c", g_no_dependencies, NULL, NULL},
	{"ft_strlcpy", "ft_strlcpy.c", g_strlen_dependencies, NULL, NULL},
	{"ft_strlcat", "ft_strlcat.c", g_strlen_dependencies, NULL, NULL},
	{"ft_toupper", "ft_toupper.c", g_no_dependencies, NULL, NULL},
	{"ft_tolower", "ft_tolower.c", g_no_dependencies, NULL, NULL},
	{"ft_strchr", "ft_strchr.c", g_no_dependencies, NULL, NULL},
	{"ft_strrchr", "ft_strrchr.c", g_no_dependencies, NULL, NULL},
	{"ft_strncmp", "ft_strncmp.c", g_no_dependencies, NULL, NULL},
	{"ft_memchr", "ft_memchr.c", g_no_dependencies, NULL, NULL},
	{"ft_memcmp", "ft_memcmp.c", g_no_dependencies, NULL, NULL},
	{"ft_strnstr", "ft_strnstr.c", g_no_dependencies, NULL, NULL},
	{"ft_atoi", "ft_atoi.c", g_no_dependencies, NULL, NULL},
	{"ft_calloc", "ft_calloc.c", g_bzero_dependencies, NULL, NULL},
	{"ft_strdup", "ft_strdup.c", g_strlen_dependencies, NULL, NULL},
	{"ft_substr", "ft_substr.c", g_strlen_dependencies, NULL, NULL},
	{"ft_strjoin", "ft_strjoin.c", g_strlen_dependencies, NULL, NULL},
	{"ft_strtrim", "ft_strtrim.c", g_strlen_dependencies, NULL, NULL},
	{"ft_split", "ft_split.c", g_strlen_dependencies, NULL, NULL},
	{"ft_itoa", "ft_itoa.c", g_no_dependencies, NULL, NULL},
	{"ft_strmapi", "ft_strmapi.c", g_strlen_dependencies, NULL, NULL},
	{"ft_striteri", "ft_striteri.c", g_no_dependencies, NULL, NULL},
	{"ft_putchar_fd", "ft_putchar_fd.c", g_no_dependencies, NULL, NULL},
	{"ft_putstr_fd", "ft_putstr_fd.c", g_strlen_dependencies, NULL, NULL},
	{"ft_putendl_fd", "ft_putendl_fd.c", g_putendl_dependencies, NULL, NULL},
	{"ft_putnbr_fd", "ft_putnbr_fd.c", g_no_dependencies, NULL, NULL}
};

const t_libft_function	*libft_function_find(const char *name)
{
	size_t	index;

	if (name == NULL)
		return (NULL);
	index = 0;
	while (index < sizeof(g_functions) / sizeof(g_functions[0]))
	{
		if (strcmp(g_functions[index].name, name) == 0)
			return (&g_functions[index]);
		index++;
	}
	return (NULL);
}

size_t	libft_function_count(void)
{
	return (sizeof(g_functions) / sizeof(g_functions[0]));
}

const char	*libft_function_name_at(size_t index)
{
	if (index >= libft_function_count())
		return (NULL);
	return (g_functions[index].name);
}

const char	*libft_function_source_name(const char *name)
{
	const t_libft_function	*function;

	function = libft_function_find(name);
	if (function == NULL)
		return (NULL);
	return (function->source_name);
}

bool	libft_function_has_tests(const char *name)
{
	const t_libft_function	*function;

	function = libft_function_find(name);
	return (function != NULL && function->test_source_path != NULL);
}
