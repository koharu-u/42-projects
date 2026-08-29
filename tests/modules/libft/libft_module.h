#ifndef LIBFT_MODULE_H
# define LIBFT_MODULE_H

# include "module.h"
# include "registry.h"

extern const t_project_module	g_libft_module;

int	libft_register_tests(t_test_registry *registry);
int	libft_build_worker(const char *target_path, t_worker_artifact *artifact,
		t_check_result *result);

#endif

