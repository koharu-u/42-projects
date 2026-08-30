#include "event.h"
#include "libft_module.h"
#include "registry.h"
#include "runner.h"
#include "scheduler.h"
#include "tester.h"
#include "wire_internal.h"
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct s_worker_options
{
	int			event_fd;
	uint64_t	seed;
	uint32_t	iterations;
	uint32_t	timeout_ms;
	const char	*test_filter;
	bool		has_iteration;
	uint64_t	iteration;
}   t_worker_options;

static int	parse_u64(const char *text, uint64_t *value)
{
	char			*end;
	unsigned long long	parsed;

	errno = 0;
	end = NULL;
	if (text[0] == '-')
		return (-1);
	parsed = strtoull(text, &end, 10);
	if (errno != 0 || end == text || *end != '\0')
		return (-1);
	*value = (uint64_t)parsed;
	return (0);
}

static int	parse_worker_options(int argc, char **argv,
		t_worker_options *options)
{
	uint64_t	value;
	int			index;

	memset(options, 0, sizeof(*options));
	options->event_fd = -1;
	options->iterations = TESTER_DEFAULT_RANDOM_ITERATIONS;
	index = 1;
	while (index < argc)
	{
		if (index + 1 >= argc || parse_u64(argv[index + 1], &value) != 0)
			return (-1);
		if (strcmp(argv[index], "--event-fd") == 0 && value <= INT32_MAX)
			options->event_fd = (int)value;
		else if (strcmp(argv[index], "--seed") == 0)
			options->seed = value;
		else if (strcmp(argv[index], "--iterations") == 0
			&& value > 0 && value <= UINT32_MAX)
			options->iterations = (uint32_t)value;
		else if (strcmp(argv[index], "--timeout") == 0
			&& value <= UINT32_MAX)
			options->timeout_ms = (uint32_t)value;
		else if (strcmp(argv[index], "--iteration") == 0)
		{
			options->has_iteration = true;
			options->iteration = value;
		}
		else if (strcmp(argv[index], "--test-index") == 0)
			return (-1);
		else
			return (-1);
		index += 2;
	}
	return (options->event_fd >= 0 ? 0 : -1);
}

static int	worker_find_test_option(int argc, char **argv,
		t_worker_options *options)
{
	int	index;

	index = 1;
	while (index < argc)
	{
		if (strcmp(argv[index], "--test") == 0)
		{
			if (index + 1 >= argc)
				return (-1);
			options->test_filter = argv[index + 1];
			index += 2;
		}
		else
			index += 2;
	}
	return (0);
}

static int	worker_parse(int argc, char **argv, t_worker_options *options)
{
	char	**filtered;
	int	filtered_count;
	int	index;
	int	status;

	if (worker_find_test_option(argc, argv, options) != 0)
		return (-1);
	filtered = calloc((size_t)argc + 1, sizeof(*filtered));
	if (filtered == NULL)
		return (-1);
	filtered[0] = argv[0];
	filtered_count = 1;
	index = 1;
	while (index < argc)
	{
		if (strcmp(argv[index], "--test") == 0)
			index += 2;
		else
		{
			filtered[filtered_count++] = argv[index++];
			if (index < argc)
				filtered[filtered_count++] = argv[index++];
		}
	}
	status = parse_worker_options(filtered_count, filtered, options);
	free(filtered);
	if (status == 0)
		status = worker_find_test_option(argc, argv, options);
	return (status);
}

static void	summary_add(t_run_summary *summary, t_test_status status)
{
	if (status == TEST_PASS)
		summary->passed++;
	else if (status == TEST_FAIL)
		summary->failed++;
	else if (status == TEST_CRASH)
		summary->crashed++;
	else if (status == TEST_TIMEOUT)
		summary->timed_out++;
	else if (status == TEST_LEAK)
		summary->leaked++;
	else if (status == TEST_SKIP)
		summary->skipped++;
	else
		summary->errors++;
}

static int	worker_run_schedule(const t_worker_options *options,
		const t_test_registry *registry)
{
	t_schedule_options	schedule_options;
	t_scheduler		scheduler;
	t_test_invocation	invocation;
	t_sandbox_options	sandbox;
	t_test_result		result;
	t_run_summary		summary;
	int				infrastructure_error;

	memset(&schedule_options, 0, sizeof(schedule_options));
	memset(&summary, 0, sizeof(summary));
	schedule_options.test_filter = options->test_filter;
	schedule_options.global_seed = options->seed;
	schedule_options.random_iterations = options->iterations;
	schedule_options.timeout_override_ms = options->timeout_ms;
	schedule_options.has_iteration = options->has_iteration;
	schedule_options.iteration = options->iteration;
	scheduler_init(&scheduler, registry, &schedule_options);
	sandbox = sandbox_default_options();
	infrastructure_error = 0;
	while (scheduler_next(&scheduler, &invocation))
	{
		if (event_wire_send_start(options->event_fd, &invocation) != 0)
			return (2);
		if (runner_execute(&invocation, &sandbox, &result) != 0)
		{
			if (result.diagnostic == NULL)
				result.diagnostic = strdup("runner could not execute test");
			if (result.input_description == NULL)
				result.input_description = strdup("");
			result.status = TEST_ERROR;
			infrastructure_error = 1;
		}
		summary_add(&summary, result.status);
		if (result.diagnostic == NULL
			|| event_wire_send_result(options->event_fd, &result) != 0)
		{
			test_result_destroy(&result);
			return (2);
		}
		test_result_destroy(&result);
	}
	if (event_wire_send_finished(options->event_fd, &summary) != 0)
		return (2);
	if (infrastructure_error || summary.errors > 0)
		return (2);
	if (summary.failed > 0 || summary.crashed > 0 || summary.timed_out > 0
		|| summary.leaked > 0)
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_worker_options	options;
	t_test_registry	registry;
	const t_test_case	*selected;
	int				status;

	if (worker_parse(argc, argv, &options) != 0)
		return (2);
	registry_init(&registry);
	if (libft_register_tests(&registry) != 0)
		return (event_wire_send_error(options.event_fd,
				"failed to register Libft tests"), registry_destroy(&registry), 2);
	registry_freeze(&registry);
	selected = NULL;
	if (options.test_filter != NULL)
		selected = registry_find(&registry, options.test_filter);
	if (options.test_filter != NULL && selected == NULL
		&& !registry_has_group(&registry, options.test_filter))
	{
		(void)event_wire_send_error(options.event_fd, "requested test not found");
		status = 2;
	}
	else if (options.has_iteration && (selected == NULL
			|| selected->type != TEST_RANDOM))
	{
		(void)event_wire_send_error(options.event_fd,
			"--iteration requires a selected random test");
		status = 2;
	}
	else
		status = worker_run_schedule(&options, &registry);
	registry_destroy(&registry);
	close(options.event_fd);
	if (status < 0)
		return (2);
	return (status);
}
