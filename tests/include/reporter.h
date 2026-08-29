#ifndef REPORTER_H
# define REPORTER_H

# include "event.h"

typedef struct s_terminal_reporter
{
	const char	*program_path;
	const char	*target_path;
}   t_terminal_reporter;

void	terminal_reporter_init(t_terminal_reporter *reporter,
			const char *program_path, const char *target_path);
void	terminal_reporter_consume(void *user_data, const t_event *event);

#endif

