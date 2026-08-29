#ifndef TIME_INTERNAL_H
# define TIME_INTERNAL_H

# include <stdbool.h>
# include <stdint.h>

uint64_t	time_now_ns(void);
bool		time_deadline_expired(uint64_t deadline_ns);
int		time_poll_timeout_ms(uint64_t deadline_ns, int maximum_ms);

#endif

