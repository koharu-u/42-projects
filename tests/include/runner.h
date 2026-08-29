#ifndef RUNNER_H
# define RUNNER_H

# include "sandbox.h"
# include "test_case.h"
# include "test_result.h"

int	runner_execute(const t_test_invocation *invocation,
		const t_sandbox_options *sandbox, t_test_result *result);

#endif

