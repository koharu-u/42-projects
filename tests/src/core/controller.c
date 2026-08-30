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

static char	*controller_validate_directory(const char *target_path)
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

static char	*controller_validate_source(const char *source_path)
{
	struct stat	information;
	char		*canonical;

	canonical = realpath(source_path, NULL);
	if (canonical == NULL)
		return (NULL);
	if (stat(canonical, &information) != 0 || !S_ISREG(information.st_mode))
	{
		free(canonical);
		return (NULL);
	}
	return (canonical);
}

static char	*controller_parent_directory(const char *path)
{
	const char	*slash;
	char		*directory;
	size_t		length;

	slash = strrchr(path, '/');
	if (slash == NULL)
		return (strdup("."));
	if (slash == path)
		return (strdup("/"));
	length = (size_t)(slash - path);
	directory = malloc(length + 1);
	if (directory == NULL)
		return (NULL);
	memcpy(directory, path, length);
	directory[length] = '\0';
	return (directory);
}

static char	*controller_join_path(const char *directory, const char *name)
{
	char	*path;
	size_t	length;

	length = strlen(directory) + strlen(name) + 2;
	path = malloc(length);
	if (path != NULL)
		(void)snprintf(path, length, "%s/%s", directory, name);
	return (path);
}

static int	controller_wait_worker(pid_t child)
{
	int		status;
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

static void	controller_worker_child(const t_build_artifact *artifact,
		const t_run_config *config, uint64_t seed, int event_pipe[2])
{
	char		fd_text[32];
	char		seed_text[32];
	char		iterations_text[32];
	char		timeout_text[32];
	char		iteration_text[32];
	char		*arguments[18];
	const char	*filter;
	int			index;

	close(event_pipe[0]);
	(void)snprintf(fd_text, sizeof(fd_text), "%d", event_pipe[1]);
	(void)snprintf(seed_text, sizeof(seed_text), "%llu",
		(unsigned long long)seed);
	(void)snprintf(iterations_text, sizeof(iterations_text), "%u",
		config->random_iterations);
	(void)snprintf(timeout_text, sizeof(timeout_text), "%u",
		config->timeout_override_ms);
	filter = config->test_filter;
	if (filter == NULL)
		filter = config->function_name;
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
	if (filter != NULL)
	{
		arguments[index++] = "--test";
		arguments[index++] = (char *)filter;
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

static int	controller_run_worker(const t_build_artifact *artifact,
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
		const char *target_path, const char *module_name, uint64_t seed,
		t_build_backend backend)
{
	t_event	event;

	memset(&event, 0, sizeof(event));
	event.type = EVENT_RUN_START;
	event.project_path = target_path;
	event.project_name = module_name;
	event.global_seed = seed;
	event.build_backend = backend;
	event_emit(sink, &event);
}

static int	controller_status_from_check(t_check_status status)
{
	if (status == CHECK_PASS || status == CHECK_SKIP || status == CHECK_MISSING
		|| status == CHECK_BLOCKED)
		return (0);
	if (status == CHECK_FAIL)
		return (1);
	return (2);
}

static int	controller_note_kept_temp(t_check_result *check,
		const t_build_artifact *artifact)
{
	char	*reason;
	size_t	length;

	if (!artifact->keep_temporary_files || artifact->session_directory == NULL)
		return (0);
	length = strlen("temporary worker kept at ")
		+ strlen(artifact->session_directory) + 1;
	if (check->reason != NULL)
		length += strlen(check->reason) + strlen("; ");
	reason = malloc(length);
	if (reason == NULL)
		return (-1);
	if (check->reason == NULL)
		(void)snprintf(reason, length, "temporary worker kept at %s",
			artifact->session_directory);
	else
		(void)snprintf(reason, length, "%s; temporary worker kept at %s",
			check->reason, artifact->session_directory);
	if (check_result_set_reason(check, reason) != 0)
		return (free(reason), -1);
	free(reason);
	return (0);
}

static int	controller_run_project(const t_project_module *module,
		const char *target_path, const t_run_config *config, uint64_t seed,
		const t_event_sink *sink)
{
	t_build_request	request;
	t_build_artifact	artifact;
	t_check_result		check;
	int				status;

	status = module->build(target_path, &check);
	if (status < 0)
		return (check_result_destroy(&check), controller_emit_error(sink,
				"build check failed internally"), 2);
	controller_emit_check(sink, EVENT_BUILD, &check);
	status = controller_status_from_check(check.status);
	check_result_destroy(&check);
	if (status != 0)
		return (controller_emit_empty_summary(sink), status);
	memset(&request, 0, sizeof(request));
	request.backend = BUILD_BACKEND_MAKE;
	request.target_path = target_path;
	request.check_name = "Harness";
	request.keep_temporary_files = config->keep_temp;
	worker_artifact_init(&artifact);
	status = module->build_worker(&request, &artifact, &check);
	if (status == 0 && check.status == CHECK_PASS
		&& controller_note_kept_temp(&check, &artifact) != 0)
		status = -1;
	controller_emit_check(sink, EVENT_HARNESS, &check);
	if (status != 0 || check.status != CHECK_PASS)
	{
		if (status >= 0)
			status = controller_status_from_check(check.status);
		check_result_destroy(&check);
		worker_artifact_destroy(&artifact);
		controller_emit_empty_summary(sink);
		return (status < 0 ? 2 : status);
	}
	check_result_destroy(&check);
	status = controller_run_worker(&artifact, config, seed, sink);
	worker_artifact_destroy(&artifact);
	return (status);
}

static int	controller_run_direct_one(const t_project_module *module,
		const char *directory, const char *source_path, const t_run_config *config,
		uint64_t seed, const t_event_sink *sink)
{
	t_build_request	request;
	t_build_artifact	artifact;
	t_check_result		check;
	int				status;

	memset(&request, 0, sizeof(request));
	request.backend = BUILD_BACKEND_DIRECT;
	request.target_path = directory;
	request.source_path = source_path;
	request.function_name = config->function_name;
	request.check_name = "Direct build";
	request.keep_temporary_files = config->keep_temp;
	worker_artifact_init(&artifact);
	status = module->build_worker(&request, &artifact, &check);
	if (status == 0 && check.status == CHECK_PASS
		&& !module->function_has_tests(config->function_name))
		(void)check_result_set_reason(&check, "compiled; no registered tests");
	if (status == 0 && check.status == CHECK_PASS
		&& controller_note_kept_temp(&check, &artifact) != 0)
		status = -1;
	controller_emit_check(sink, EVENT_BUILD, &check);
	if (status != 0 || check.status != CHECK_PASS)
	{
		status = (status < 0) ? 2 : controller_status_from_check(check.status);
		check_result_destroy(&check);
		worker_artifact_destroy(&artifact);
		controller_emit_empty_summary(sink);
		return (status);
	}
	check_result_destroy(&check);
	if (!module->function_has_tests(config->function_name))
	{
		worker_artifact_destroy(&artifact);
		controller_emit_empty_summary(sink);
		return (0);
	}
	status = controller_run_worker(&artifact, config, seed, sink);
	worker_artifact_destroy(&artifact);
	return (status);
}

static int	controller_run_partial(const t_project_module *module,
		const char *target_path, const t_run_config *config, uint64_t seed,
		const t_event_sink *sink)
{
	t_build_request	request;
	t_build_artifact	artifact;
	t_check_result		check;
	const char		*function_name;
	const char		*source_name;
	char				*source_path;
	size_t				index;
	int					status;
	int					result_status;
	bool				ran_tests;

	if (config->function_name != NULL
		&& module->function_source_name(config->function_name) == NULL)
		return (controller_emit_error(sink, "requested Libft function is unknown"), 2);
	result_status = 0;
	ran_tests = false;
	index = 0;
	while (index < module->function_count())
	{
		function_name = module->function_name_at(index++);
		if (config->function_name != NULL
			&& strcmp(function_name, config->function_name) != 0)
			continue ;
		source_name = module->function_source_name(function_name);
		source_path = controller_join_path(target_path, source_name);
		if (source_path == NULL)
			return (controller_emit_error(sink,
					"could not resolve a partial-project source path"), 2);
		memset(&request, 0, sizeof(request));
		request.backend = BUILD_BACKEND_DIRECT;
		request.target_path = target_path;
		request.source_path = source_path;
		request.function_name = function_name;
		request.check_name = function_name;
		request.keep_temporary_files = config->keep_temp;
		worker_artifact_init(&artifact);
		status = module->build_worker(&request, &artifact, &check);
		free(source_path);
		if (status == 0 && check.status == CHECK_PASS
			&& !module->function_has_tests(function_name))
			(void)check_result_set_reason(&check, "compiled; no registered tests");
		if (status == 0 && check.status == CHECK_PASS
			&& controller_note_kept_temp(&check, &artifact) != 0)
			status = -1;
		controller_emit_check(sink, EVENT_BUILD, &check);
		if (status != 0 || check.status != CHECK_PASS)
		{
			if (status < 0)
				result_status = 2;
			else if (result_status == 0)
				result_status = controller_status_from_check(check.status);
			check_result_destroy(&check);
			worker_artifact_destroy(&artifact);
			if (result_status == 2)
				return (controller_emit_empty_summary(sink), 2);
			continue ;
		}
		check_result_destroy(&check);
		if (module->function_has_tests(function_name))
		{
			ran_tests = true;
			status = controller_run_worker(&artifact, config, seed, sink);
			if (status > result_status)
				result_status = status;
		}
		worker_artifact_destroy(&artifact);
		if (result_status == 2)
			return (2);
	}
	if (!ran_tests)
		controller_emit_empty_summary(sink);
	return (result_status);
}

int	controller_run(const t_run_config *config, const t_event_sink *sink)
{
	char				*target_path;
	char				*source_path;
	const t_project_module	*module;
	t_check_result			check;
	t_build_backend			backend;
	uint64_t				seed;
	t_check_status			norm_status;
	int					status;

	source_path = NULL;
	target_path = NULL;
	if (config->source_path != NULL)
	{
		source_path = controller_validate_source(config->source_path);
		if (source_path != NULL)
			target_path = controller_parent_directory(source_path);
	}
	else
		target_path = controller_validate_directory(config->target_path);
	if (target_path == NULL || (config->source_path != NULL && source_path == NULL))
		return (free(source_path), controller_emit_error(sink,
				"target path does not name an accessible directory or source file"), 2);
	module = module_select(config->project_name, target_path);
	if (module == NULL && config->source_path != NULL
		&& config->project_name == NULL && config->function_name != NULL)
		module = module_select("libft", target_path);
	if (module == NULL)
		return (free(source_path), free(target_path), controller_emit_error(sink,
				"could not select a supported project module"), 2);
	backend = BUILD_BACKEND_MAKE;
	if (config->source_path != NULL || config->partial)
		backend = BUILD_BACKEND_DIRECT;
	seed = config->seed;
	if (!config->seed_was_set)
		seed = rng_auto_seed();
	controller_emit_run_start(sink, source_path != NULL ? source_path : target_path,
		module->name, seed, backend);
	status = preflight_run_norminette(source_path != NULL ? source_path : target_path,
			&check);
	if (status != 0)
	{
		check_result_destroy(&check);
		free(source_path);
		free(target_path);
		return (controller_emit_error(sink, "Norminette preflight failed internally"), 2);
	}
	norm_status = check.status;
	controller_emit_check(sink, EVENT_PREFLIGHT, &check);
	check_result_destroy(&check);
	if (backend == BUILD_BACKEND_MAKE)
		status = controller_run_project(module, target_path, config, seed, sink);
	else if (config->partial)
		status = controller_run_partial(module, target_path, config, seed, sink);
	else
		status = controller_run_direct_one(module, target_path, source_path, config,
				seed, sink);
	free(source_path);
	free(target_path);
	if (status == 0 && norm_status == CHECK_FAIL)
		return (1);
	return (status);
}
