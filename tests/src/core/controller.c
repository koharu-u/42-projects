#include "controller.h"
#include "module.h"
#include "preflight.h"
#include "rng.h"
#include "wire_internal.h"
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static void	controller_emit_error(const t_event_sink *sink,
		const char *message)
{
	t_event	event;

	memset(&event, 0, sizeof(event));
	event.type = EVENT_ERROR;
	event.message = message;
	event_emit(sink, &event);
}

static void	controller_emit_check(const t_event_sink *sink,
		t_event_type type, const t_check_result *check)
{
	t_event	event;

	memset(&event, 0, sizeof(event));
	event.type = type;
	event.check = check;
	event_emit(sink, &event);
}

static void	controller_emit_empty_summary(const t_event_sink *sink)
{
	t_run_summary	summary;
	t_event		event;

	memset(&summary, 0, sizeof(summary));
	memset(&event, 0, sizeof(event));
	event.type = EVENT_FINISHED;
	event.summary = &summary;
	event_emit(sink, &event);
}

static char	*controller_validate_target(const char *target_path)
{
	struct stat	information;
	char		*canonical;

	canonical = realpath(target_path, NULL);
	if (canonical == NULL)
		return (NULL);
	if (stat(canonical, &information) != 0 || !S_ISDIR(information.st_mode))
	{
		free(canonical);
		return (NULL);
	}
	return (canonical);
}

static int	controller_wait_worker(pid_t child)
{
	int	status;
	pid_t	waited;

	while (true)
	{
		waited = waitpid(child, &status, 0);
		if (waited == child)
			break ;
		if (waited < 0 && errno == EINTR)
			continue ;
		return (2);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (2);
}

static void	controller_worker_child(const t_worker_artifact *artifact,
		const t_run_config *config, uint64_t seed, int event_pipe[2])
{
	char	fd_text[32];
	char	seed_text[32];
	char	iterations_text[32];
	char	timeout_text[32];
	char	iteration_text[32];
	char	*arguments[18];
	int	index;

	close(event_pipe[0]);
	(void)snprintf(fd_text, sizeof(fd_text), "%d", event_pipe[1]);
	(void)snprintf(seed_text, sizeof(seed_text), "%llu",
		(unsigned long long)seed);
	(void)snprintf(iterations_text, sizeof(iterations_text), "%u",
		config->random_iterations);
	(void)snprintf(timeout_text, sizeof(timeout_text), "%u",
		config->timeout_override_ms);
	index = 0;
	arguments[index++] = artifact->executable_path;
	arguments[index++] = "--event-fd";
	arguments[index++] = fd_text;
	arguments[index++] = "--seed";
	arguments[index++] = seed_text;
	arguments[index++] = "--iterations";
	arguments[index++] = iterations_text;
	arguments[index++] = "--timeout";
	arguments[index++] = timeout_text;
	if (config->test_filter != NULL)
	{
		arguments[index++] = "--test";
		arguments[index++] = (char *)config->test_filter;
	}
	if (config->has_iteration)
	{
		(void)snprintf(iteration_text, sizeof(iteration_text), "%llu",
			(unsigned long long)config->iteration);
		arguments[index++] = "--iteration";
		arguments[index++] = iteration_text;
	}
	arguments[index] = NULL;
	execv(artifact->executable_path, arguments);
	(void)event_wire_send_error(event_pipe[1],
		"could not execute the target-linked worker");
	close(event_pipe[1]);
	_exit(127);
}

static int	controller_run_worker(const t_worker_artifact *artifact,
		const t_run_config *config, uint64_t seed, const t_event_sink *sink)
{
	int		event_pipe[2];
	pid_t	child;
	int		protocol_status;
	int		worker_status;

	if (pipe(event_pipe) != 0)
		return (controller_emit_error(sink,
				"could not create the worker event pipe"), 2);
	child = fork();
	if (child < 0)
	{
		close(event_pipe[0]);
		close(event_pipe[1]);
		return (controller_emit_error(sink,
				"could not start the target-linked worker"), 2);
	}
	if (child == 0)
		controller_worker_child(artifact, config, seed, event_pipe);
	close(event_pipe[1]);
	protocol_status = event_wire_receive_all(event_pipe[0], sink);
	close(event_pipe[0]);
	worker_status = controller_wait_worker(child);
	if (protocol_status != 0)
	{
		controller_emit_error(sink, "worker sent an invalid event stream");
		return (2);
	}
	if (worker_status > 2)
		return (2);
	return (worker_status);
}

static void	controller_emit_run_start(const t_event_sink *sink,
		const char *target_path, const char *module_name, uint64_t seed)
{
	t_event	event;

	memset(&event, 0, sizeof(event));
	event.type = EVENT_RUN_START;
	event.project_path = target_path;
	event.project_name = module_name;
	event.global_seed = seed;
	event_emit(sink, &event);
}

int	controller_run(const t_run_config *config, const t_event_sink *sink)
{
	char				*target_path;
	const t_project_module	*module;
	uint64_t			seed;
	t_check_result		check;
	t_worker_artifact	artifact;
	t_check_status		norm_status;
	int				status;

	target_path = controller_validate_target(config->target_path);
	if (target_path == NULL)
		return (controller_emit_error(sink,
				"target path does not name an accessible directory"), 2);
	module = module_select(config->project_name, target_path);
	if (module == NULL)
		return (free(target_path), controller_emit_error(sink,
				"could not select a supported project module"), 2);
	seed = config->seed;
	if (!config->seed_was_set)
		seed = rng_auto_seed();
	controller_emit_run_start(sink, target_path, module->name, seed);
	status = preflight_run_norminette(target_path, &check);
	if (status != 0)
	{
		check_result_destroy(&check);
		free(target_path);
		controller_emit_error(sink, "Norminette preflight failed internally");
		return (2);
	}
	norm_status = check.status;
	controller_emit_check(sink, EVENT_PREFLIGHT, &check);
	check_result_destroy(&check);
	status = module->build(target_path, &check);
	if (status < 0)
	{
		check_result_destroy(&check);
		free(target_path);
		controller_emit_error(sink, "build check failed internally");
		return (2);
	}
	controller_emit_check(sink, EVENT_BUILD, &check);
	if (check.status != CHECK_PASS)
	{
		status = (check.status == CHECK_FAIL) ? 1 : 2;
		check_result_destroy(&check);
		controller_emit_empty_summary(sink);
		free(target_path);
		return (status);
	}
	check_result_destroy(&check);
	worker_artifact_init(&artifact);
	status = module->build_worker(target_path, &artifact, &check);
	controller_emit_check(sink, EVENT_HARNESS, &check);
	if (status != 0 || check.status != CHECK_PASS)
	{
		check_result_destroy(&check);
		worker_artifact_destroy(&artifact);
		controller_emit_empty_summary(sink);
		free(target_path);
		return (status < 0 ? 2 : 1);
	}
	check_result_destroy(&check);
	status = controller_run_worker(&artifact, config, seed, sink);
	worker_artifact_destroy(&artifact);
	free(target_path);
	if (status == 0 && norm_status == CHECK_FAIL)
		return (1);
	return (status);
}
