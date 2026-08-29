#ifndef TESTER_H
# define TESTER_H

# include <stddef.h>
# include <stdint.h>

# define TESTER_VERSION "0.1.0-phase1"
# define TESTER_DEFAULT_TIMEOUT_MS 1000U
# define TESTER_DEFAULT_RANDOM_ITERATIONS 100U
# define TESTER_TOOL_TIMEOUT_MS 30000U
# define TESTER_BUILD_TIMEOUT_MS 120000U
# define TESTER_CAPTURE_LIMIT (256U * 1024U)
# define TESTER_DIAGNOSTIC_CAP 2048U
# define TESTER_INPUT_DESCRIPTION_CAP 1024U

typedef struct s_run_summary
{
	uint64_t	passed;
	uint64_t	failed;
	uint64_t	crashed;
	uint64_t	timed_out;
	uint64_t	leaked;
	uint64_t	skipped;
	uint64_t	errors;
}   t_run_summary;

#endif
