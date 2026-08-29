#include "scheduler.h"
#include "tester.h"
#include <string.h>

void	scheduler_init(t_scheduler *scheduler,
		const t_test_registry *registry, const t_schedule_options *options)
{
	memset(scheduler, 0, sizeof(*scheduler));
	scheduler->registry = registry;
	scheduler->options = *options;
}

static bool	scheduler_matches(const t_scheduler *scheduler,
		const t_test_case *test)
{
	return (scheduler->options.test_filter == NULL
		|| strcmp(scheduler->options.test_filter, test->id) == 0);
}

static uint64_t	scheduler_random_iteration(t_scheduler *scheduler,
		const t_test_case *test, bool *available)
{
	uint64_t	count;

	if (scheduler->options.has_iteration)
	{
		if (scheduler->single_iteration_emitted)
		{
			*available = false;
			return (0);
		}
		scheduler->single_iteration_emitted = true;
		return (scheduler->options.iteration);
	}
	count = scheduler->options.random_iterations;
	if (count == 0)
		count = test->default_iterations;
	if (scheduler->random_index >= count)
	{
		*available = false;
		return (0);
	}
	return (scheduler->random_index++);
}

bool	scheduler_next(t_scheduler *scheduler, t_test_invocation *invocation)
{
	const t_test_case	*test;
	uint64_t			iteration;
	bool				available;

	while (scheduler->case_index < registry_count(scheduler->registry))
	{
		test = registry_at(scheduler->registry, scheduler->case_index);
		if (!scheduler_matches(scheduler, test)
			|| (scheduler->options.has_iteration && test->type != TEST_RANDOM))
		{
			scheduler->case_index++;
			continue ;
		}
		iteration = 0;
		if (test->type == TEST_RANDOM)
		{
			available = true;
			iteration = scheduler_random_iteration(scheduler, test, &available);
			if (!available)
			{
				scheduler->case_index++;
				scheduler->random_index = 0;
				continue ;
			}
		}
		else
			scheduler->case_index++;
		invocation->test = test;
		invocation->global_seed = scheduler->options.global_seed;
		invocation->iteration = iteration;
		invocation->test_seed = rng_derive_seed(invocation->global_seed,
				test->id, iteration);
		invocation->timeout_ms = test->timeout_ms;
		if (invocation->timeout_ms == 0)
			invocation->timeout_ms = TESTER_DEFAULT_TIMEOUT_MS;
		if (scheduler->options.timeout_override_ms != 0)
			invocation->timeout_ms = scheduler->options.timeout_override_ms;
		return (true);
	}
	return (false);
}

