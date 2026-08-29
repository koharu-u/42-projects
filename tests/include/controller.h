#ifndef CONTROLLER_H
# define CONTROLLER_H

# include "event.h"
# include <stdbool.h>
# include <stdint.h>

typedef struct s_run_config
{
	const char	*target_path;
	const char	*project_name;
	const char	*test_filter;
	uint64_t	seed;
	uint32_t	random_iterations;
	uint32_t	timeout_override_ms;
	bool		seed_was_set;
	bool		no_ui;
	bool		has_iteration;
	uint64_t	iteration;
}   t_run_config;

int	controller_run(const t_run_config *config, const t_event_sink *sink);

#endif

