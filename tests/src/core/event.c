#include "event.h"

void	event_emit(const t_event_sink *sink, const t_event *event)
{
	if (sink != NULL && sink->consume != NULL)
		sink->consume(sink->user_data, event);
}

