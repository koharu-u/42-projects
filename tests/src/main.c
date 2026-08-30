#include "controller.h"
#include "reporter.h"
#include "tester.h"
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void	print_usage(const char *program)
{
	(void)printf("Usage: %s [options] <project-path>\n", program);
	(void)printf("       %s --source <file.c> --function <name> [options]\n",
		program);
	(void)printf("\nOptions:\n");
	(void)printf("  --project libft       Select the Libft module\n");
	(void)printf("  --source FILE         Directly compile one C source file\n");
	(void)printf("  --function NAME       Select a Libft function\n");
	(void)printf("  --partial             Test available Libft sources without make\n");
	(void)printf("  --keep-temp           Keep the temporary direct-build worker\n");
	(void)printf("  --test ID             Run one registered test\n");
	(void)printf("  --seed N              Set the deterministic global seed\n");
	(void)printf("  --iterations N        Random iterations (default: %u)\n",
		TESTER_DEFAULT_RANDOM_ITERATIONS);
	(void)printf("  --iteration N         Replay one random iteration\n");
	(void)printf("  --timeout MS          Override each test timeout\n");
	(void)printf("  --no-ui               Use the Phase 1 headless frontend\n");
	(void)printf("  --help                Show this help\n");
}

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

static int	option_value(int argc, char **argv, int *index,
		const char **value)
{
	if (*index + 1 >= argc)
		return (-1);
	*value = argv[*index + 1];
	*index += 2;
	return (0);
}

static int	parse_numeric_option(const char *name, const char *value,
		t_run_config *config)
{
	uint64_t	parsed;

	if (parse_u64(value, &parsed) != 0)
		return (-1);
	if (strcmp(name, "--seed") == 0)
	{
		config->seed = parsed;
		config->seed_was_set = true;
	}
	else if (strcmp(name, "--iterations") == 0
		&& parsed > 0 && parsed <= UINT32_MAX)
		config->random_iterations = (uint32_t)parsed;
	else if (strcmp(name, "--timeout") == 0
		&& parsed > 0 && parsed <= UINT32_MAX)
		config->timeout_override_ms = (uint32_t)parsed;
	else if (strcmp(name, "--iteration") == 0)
	{
		config->has_iteration = true;
		config->iteration = parsed;
	}
	else
		return (-1);
	return (0);
}

static int	parse_option(int argc, char **argv, int *index,
		t_run_config *config)
{
	const char	*name;
	const char	*value;

	name = argv[*index];
	if (strcmp(name, "--no-ui") == 0)
	{
		config->no_ui = true;
		(*index)++;
		return (0);
	}
	if (strcmp(name, "--partial") == 0)
	{
		config->partial = true;
		(*index)++;
		return (0);
	}
	if (strcmp(name, "--keep-temp") == 0)
	{
		config->keep_temp = true;
		(*index)++;
		return (0);
	}
	if (option_value(argc, argv, index, &value) != 0)
		return (-1);
	if (strcmp(name, "--project") == 0)
		config->project_name = value;
	else if (strcmp(name, "--test") == 0)
		config->test_filter = value;
	else if (strcmp(name, "--source") == 0)
		config->source_path = value;
	else if (strcmp(name, "--function") == 0)
		config->function_name = value;
	else
		return (parse_numeric_option(name, value, config));
	return (0);
}

static int	parse_arguments(int argc, char **argv, t_run_config *config)
{
	int	index;

	memset(config, 0, sizeof(*config));
	config->random_iterations = TESTER_DEFAULT_RANDOM_ITERATIONS;
	index = 1;
	while (index < argc)
	{
		if (strcmp(argv[index], "--help") == 0)
			return (1);
		if (argv[index][0] == '-')
		{
			if (parse_option(argc, argv, &index, config) != 0)
				return (-1);
		}
		else
		{
			if (config->target_path != NULL)
				return (-1);
			config->target_path = argv[index++];
		}
	}
	if ((config->target_path == NULL && config->source_path == NULL)
		|| (config->target_path != NULL && config->source_path != NULL)
		|| (config->source_path != NULL && config->function_name == NULL)
		|| (config->partial && config->target_path == NULL)
		|| (config->has_iteration && config->test_filter == NULL))
		return (-1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_run_config		config;
	t_terminal_reporter	reporter;
	t_event_sink		sink;
	int				status;

	status = parse_arguments(argc, argv, &config);
	if (status != 0)
	{
		print_usage(argv[0]);
		return (status < 0 ? 2 : 0);
	}
	terminal_reporter_init(&reporter, argv[0], config.target_path);
	sink.consume = terminal_reporter_consume;
	sink.user_data = &reporter;
	return (controller_run(&config, &sink));
}
