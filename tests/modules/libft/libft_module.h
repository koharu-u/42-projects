#ifndef LIBFT_MODULE_H
# define LIBFT_MODULE_H

# include "module.h"
# include "registry.h"
# include <stddef.h>

typedef struct s_libft_function
{
	const char		*name;
	const char		*source_name;
	const char *const	*dependencies;
	const char		*test_source_path;
	const char		*test_define;
}   t_libft_function;

extern const t_project_module	g_libft_module;

int	libft_register_tests(t_test_registry *registry);
const t_libft_function	*libft_function_find(const char *name);
size_t				libft_function_count(void);
const char				*libft_function_name_at(size_t index);
const char				*libft_function_source_name(const char *name);
bool				libft_function_has_tests(const char *name);
int	libft_build_worker(const t_build_request *request,
			t_build_artifact *artifact, t_check_result *result);

#endif
