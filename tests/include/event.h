#ifndef EVENT_H
# define EVENT_H

# include "module.h"
# include "test_case.h"
# include "test_result.h"
# include "tester.h"
# include <stdint.h>

typedef enum e_event_type
{
	EVENT_RUN_START,
	EVENT_PREFLIGHT,
	EVENT_BUILD,
	EVENT_HARNESS,
	EVENT_TEST_START,
	EVENT_TEST_RESULT,
	EVENT_PROGRESS,
	EVENT_FINISHED,
	EVENT_ERROR
}   t_event_type;

typedef struct s_event
{
	t_event_type			type;
	const char			*project_path;
	const char			*project_name;
	t_build_backend		build_backend;
	uint64_t			global_seed;
	const t_check_result		*check;
	const t_test_invocation	*invocation;
	const t_test_result		*result;
	const t_run_summary		*summary;
	const char			*message;
}   t_event;

typedef void	(*t_event_consumer)(void *user_data, const t_event *event);

typedef struct s_event_sink
{
	t_event_consumer	consume;
	void			*user_data;
}   t_event_sink;

void	event_emit(const t_event_sink *sink, const t_event *event);

#endif
