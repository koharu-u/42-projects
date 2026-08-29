#include "rng.h"
#include <fcntl.h>
#include <stddef.h>
#include <time.h>
#include <unistd.h>

static uint64_t	splitmix64(uint64_t value)
{
	value += 0x9e3779b97f4a7c15ULL;
	value = (value ^ (value >> 30)) * 0xbf58476d1ce4e5b9ULL;
	value = (value ^ (value >> 27)) * 0x94d049bb133111ebULL;
	return (value ^ (value >> 31));
}

uint32_t	rng_next_u32(t_rng *rng)
{
	uint64_t	old_state;
	uint32_t	shifted;
	uint32_t	rotation;

	old_state = rng->state;
	rng->state = old_state * 6364136223846793005ULL + rng->increment;
	shifted = (uint32_t)(((old_state >> 18U) ^ old_state) >> 27U);
	rotation = (uint32_t)(old_state >> 59U);
	return ((shifted >> rotation) | (shifted << ((-rotation) & 31U)));
}

void	rng_init(t_rng *rng, uint64_t seed)
{
	rng->state = 0;
	rng->increment = (splitmix64(seed) << 1U) | 1U;
	(void)rng_next_u32(rng);
	rng->state += splitmix64(seed ^ 0xda3e39cb94b95bdbULL);
	(void)rng_next_u32(rng);
}

uint64_t	rng_next_u64(t_rng *rng)
{
	return (((uint64_t)rng_next_u32(rng) << 32U) | rng_next_u32(rng));
}

uint64_t	rng_range(t_rng *rng, uint64_t upper_bound)
{
	uint64_t	value;
	uint64_t	threshold;

	if (upper_bound == 0)
		return (0);
	threshold = (uint64_t)(-upper_bound) % upper_bound;
	value = rng_next_u64(rng);
	while (value < threshold)
		value = rng_next_u64(rng);
	return (value % upper_bound);
}

uint64_t	rng_derive_seed(uint64_t global_seed, const char *test_id,
		uint64_t iteration)
{
	uint64_t	hash;
	size_t		index;

	hash = 1469598103934665603ULL;
	index = 0;
	while (test_id[index] != '\0')
	{
		hash ^= (unsigned char)test_id[index];
		hash *= 1099511628211ULL;
		index++;
	}
	return (splitmix64(global_seed ^ hash ^ splitmix64(iteration)));
}

uint64_t	rng_auto_seed(void)
{
	struct timespec	time_value;
	uint64_t		seed;
	ssize_t			count;
	int				fd;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd >= 0)
	{
		count = read(fd, &seed, sizeof(seed));
		close(fd);
		if (count == (ssize_t)sizeof(seed))
			return (seed);
	}
	clock_gettime(CLOCK_REALTIME, &time_value);
	seed = (uint64_t)time_value.tv_sec ^ ((uint64_t)time_value.tv_nsec << 32U);
	seed ^= (uint64_t)getpid() << 16U;
	return (splitmix64(seed));
}

