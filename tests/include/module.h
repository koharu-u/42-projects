#ifndef MODULE_H
# define MODULE_H

# include "preflight.h"
# include <stdbool.h>
# include <stddef.h>

typedef enum e_build_backend
{
	BUILD_BACKEND_MAKE,
	BUILD_BACKEND_DIRECT
}   t_build_backend;

typedef struct s_build_artifact
{
	char	*session_directory;
	char	*executable_path;
	t_build_backend	backend;
	bool	keep_temporary_files;
}   t_build_artifact;

typedef t_build_artifact	t_worker_artifact;

typedef struct s_build_request
{
	t_build_backend	backend;
	const char		*target_path;
	const char		*source_path;
	const char		*function_name;
	const char		*check_name;
	bool			keep_temporary_files;
}   t_build_request;

typedef struct s_worker_build_plan
{
	const char		*worker_name;
	const char *const	*source_paths;
	size_t			source_count;
	const char *const	*include_paths;
	size_t			include_count;
	const char *const	*external_include_paths;
	size_t			external_include_count;
	const char *const	*target_source_paths;
	size_t			target_source_count;
	const char *const	*defines;
	size_t			define_count;
	const char		*target_artifact_name;
	const char		*check_name;
	t_build_backend		backend;
	bool			keep_temporary_files;
}   t_worker_build_plan;

typedef struct s_project_module
{
	const char	*name;
	bool		(*probe)(const char *target_path);
	int			(*build)(const char *target_path,
					t_check_result *result);
	int			(*build_worker)(const t_build_request *request,
					t_build_artifact *artifact, t_check_result *result);
	size_t		(*function_count)(void);
	const char	*(*function_name_at)(size_t index);
	const char	*(*function_source_name)(const char *function_name);
	bool		(*function_has_tests)(const char *function_name);
}   t_project_module;

const t_project_module	*module_select(const char *requested_name,
						const char *target_path);
void			worker_artifact_init(t_build_artifact *artifact);
void			worker_artifact_destroy(t_build_artifact *artifact);
int			worker_build_from_plan(const char *target_path,
						const t_worker_build_plan *plan,
						t_build_artifact *artifact, t_check_result *result);

#endif
