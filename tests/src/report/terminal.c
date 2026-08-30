#include "reporter.h"
#include "test_result.h"
#include "tester.h"
#include <stdio.h>
#include <string.h>

void	terminal_reporter_init(t_terminal_reporter *reporter,
		const char *program_path, const char *target_path)
{
	reporter->program_path = program_path;
	reporter->target_path = target_path;
}

static void	terminal_print_bytes(const char *label,
		const unsigned char *data, size_t length, bool truncated)
{
	if (length == 0 && !truncated)
		return ;
	(void)printf("  %s:\n", label);
	if (length > 0)
		(void)fwrite(data, 1, length, stdout);
	if (length > 0 && data[length - 1] != '\n')
		(void)putchar('\n');
	if (truncated)
		(void)printf("  [... output truncated ...]\n");
}

static void	terminal_print_check(const t_check_result *check)
{
	(void)printf("%s: %s", check->name, check_status_name(check->status));
	if (check->reason != NULL)
		(void)printf(" (%s)", check->reason);
	(void)putchar('\n');
	terminal_print_bytes("stdout", check->stdout_data, check->stdout_length,
		check->stdout_truncated);
	terminal_print_bytes("stderr", check->stderr_data, check->stderr_length,
		check->stderr_truncated);
}

static bool	terminal_is_random(const char *test_id)
{
	size_t	length;

	length = strlen(test_id);
	return (length >= 7 && strcmp(test_id + length - 7, ".random") == 0);
}

static void	terminal_print_result(t_terminal_reporter *reporter,
		const t_test_result *result)
{
	double	milliseconds;

	milliseconds = (double)result->duration_ns / 1000000.0;
	(void)printf("  %s %s", test_status_name(result->status), result->test_id);
	if (terminal_is_random(result->test_id))
		(void)printf(" iteration=%llu test_seed=%llu",
			(unsigned long long)result->iteration,
			(unsigned long long)result->test_seed);
	(void)printf(" (%.3f ms)\n", milliseconds);
	if (result->signal_number != 0)
		(void)printf("    signal: %s (%d)\n",
			test_signal_name(result->signal_number), result->signal_number);
	if (result->diagnostic != NULL && result->diagnostic[0] != '\0')
		(void)printf("    diagnostic: %s\n", result->diagnostic);
	if (result->status != TEST_PASS && result->input_description != NULL
		&& result->input_description[0] != '\0')
		(void)printf("    input: %s\n", result->input_description);
	terminal_print_bytes("captured stdout", result->stdout_data,
		result->stdout_length, result->stdout_truncated);
	terminal_print_bytes("captured stderr", result->stderr_data,
		result->stderr_length, result->stderr_truncated);
	if (terminal_is_random(result->test_id) && result->status != TEST_PASS)
	{
		(void)printf("    global seed: %llu\n",
			(unsigned long long)result->global_seed);
		(void)printf("    test seed: %llu\n",
			(unsigned long long)result->test_seed);
		(void)printf("    iteration: %llu\n",
			(unsigned long long)result->iteration);
		(void)printf("    replay: %s --project libft --test %s --seed %llu "
			"--iteration %llu %s\n", reporter->program_path,
			result->test_id, (unsigned long long)result->global_seed,
			(unsigned long long)result->iteration, reporter->target_path);
	}
}

static void	terminal_print_summary(const t_run_summary *summary)
{
	(void)printf("\nSummary:\n");
	(void)printf("%llu PASS\n", (unsigned long long)summary->passed);
	(void)printf("%llu FAIL\n", (unsigned long long)summary->failed);
	(void)printf("%llu CRASH\n", (unsigned long long)summary->crashed);
	(void)printf("%llu TIMEOUT\n", (unsigned long long)summary->timed_out);
	(void)printf("%llu LEAK\n", (unsigned long long)summary->leaked);
	(void)printf("%llu SKIP\n", (unsigned long long)summary->skipped);
	(void)printf("%llu ERROR\n", (unsigned long long)summary->errors);
}

void	terminal_reporter_consume(void *user_data, const t_event *event)
{
	t_terminal_reporter	*reporter;

	reporter = user_data;
	if (event->type == EVENT_RUN_START)
	{
		(void)printf("42tester %s\n", TESTER_VERSION);
		(void)printf("Project: %s\n", event->project_path);
		(void)printf("Module: %s\n", event->project_name);
		(void)printf("Build mode: %s\n", event->build_backend == BUILD_BACKEND_DIRECT
			? "DIRECT / PARTIAL" : "PROJECT");
		(void)printf("Seed: %llu\n\n",
			(unsigned long long)event->global_seed);
	}
	else if (event->type == EVENT_PREFLIGHT || event->type == EVENT_BUILD
		|| event->type == EVENT_HARNESS)
		terminal_print_check(event->check);
	else if (event->type == EVENT_TEST_RESULT)
		terminal_print_result(reporter, event->result);
	else if (event->type == EVENT_FINISHED)
		terminal_print_summary(event->summary);
	else if (event->type == EVENT_ERROR)
		(void)fprintf(stderr, "42tester: %s\n", event->message);
}
