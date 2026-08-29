#include "time_internal.h"
#include <limits.h>
#include <time.h>

uint64_t	time_now_ns(void)
{
	struct timespec	value;

	if (clock_gettime(CLOCK_MONOTONIC, &value) != 0)
		return (0);
	return ((uint64_t)value.tv_sec * 1000000000ULL
		+ (uint64_t)value.tv_nsec);
}

bool	time_deadline_expired(uint64_t deadline_ns)
{
	return (time_now_ns() >= deadline_ns);
}

int	time_poll_timeout_ms(uint64_t deadline_ns, int maximum_ms)
{
	uint64_t	now;
	uint64_t	remaining;
	uint64_t	milliseconds;

	now = time_now_ns();
	if (now >= deadline_ns)
		return (0);
	remaining = deadline_ns - now;
	milliseconds = (remaining + 999999ULL) / 1000000ULL;
	if (milliseconds > (uint64_t)maximum_ms)
		return (maximum_ms);
	if (milliseconds > INT_MAX)
		return (INT_MAX);
	return ((int)milliseconds);
}

