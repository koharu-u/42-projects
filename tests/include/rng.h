#ifndef RNG_H
# define RNG_H

# include <stdint.h>

typedef struct s_rng
{
	uint64_t	state;
	uint64_t	increment;
}   t_rng;

void		rng_init(t_rng *rng, uint64_t seed);
uint32_t	rng_next_u32(t_rng *rng);
uint64_t	rng_next_u64(t_rng *rng);
uint64_t	rng_range(t_rng *rng, uint64_t upper_bound);
uint64_t	rng_derive_seed(uint64_t global_seed, const char *test_id,
				uint64_t iteration);
uint64_t	rng_auto_seed(void);

#endif

