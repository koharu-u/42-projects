#ifndef MODULE_H
# define MODULE_H

# include "preflight.h"
# include <stdbool.h>
# include <stddef.h>

typedef struct s_worker_artifact
{
	char	*session_directory;
	char	*executable_path;
}   t_worker_artifact;

typedef struct s_worker_build_plan
{
	const char		*worker_name;
	const char *const	*source_paths;
	size_t			source_count;
	const char *const	*include_paths;
	size_t			include_count;
	const char		*target_artifact_name;
}   t_worker_build_plan;

typedef struct s_project_module
{
	const char	*name;
	bool		(*probe)(const char *target_path);
	int			(*build)(const char *target_path,
					t_check_result *result);
	int			(*build_worker)(const char *target_path,
					t_worker_artifact *artifact, t_check_result *result);
}   t_project_module;

const t_project_module	*module_select(const char *requested_name,
						const char *target_path);
void			worker_artifact_init(t_worker_artifact *artifact);
void			worker_artifact_destroy(t_worker_artifact *artifact);
int			worker_build_from_plan(const char *target_path,
						const t_worker_build_plan *plan,
						t_worker_artifact *artifact, t_check_result *result);

#endif
