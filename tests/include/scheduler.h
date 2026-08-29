#ifndef SCHEDULER_H
# define SCHEDULER_H

# include "registry.h"
# include <stdbool.h>
# include <stdint.h>

typedef struct s_schedule_options
{
	const char	*test_filter;
	uint64_t	global_seed;
	uint32_t	random_iterations;
	uint32_t	timeout_override_ms;
	bool		has_iteration;
	uint64_t	iteration;
}   t_schedule_options;

typedef struct s_scheduler
{
	const t_test_registry	*registry;
	t_schedule_options	options;
	size_t			case_index;
	uint64_t			random_index;
	bool				single_iteration_emitted;
}   t_scheduler;

void	scheduler_init(t_scheduler *scheduler,
			const t_test_registry *registry, const t_schedule_options *options);
bool	scheduler_next(t_scheduler *scheduler, t_test_invocation *invocation);

#endif

