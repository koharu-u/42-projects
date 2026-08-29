#ifndef SANDBOX_H
# define SANDBOX_H

# include "tester.h"
# include <stddef.h>

typedef struct s_sandbox_options
{
	size_t	capture_limit;
}   t_sandbox_options;

static inline t_sandbox_options	sandbox_default_options(void)
{
	t_sandbox_options	options;

	options.capture_limit = TESTER_CAPTURE_LIMIT;
	return (options);
}

#endif

