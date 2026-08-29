#include "module.h"
#include "process_internal.h"
#include "tester.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef TESTER_SOURCE_ROOT
# error "TESTER_SOURCE_ROOT must be defined by the tests Makefile"
#endif

void	worker_artifact_init(t_worker_artifact *artifact)
{
	artifact->session_directory = NULL;
	artifact->executable_path = NULL;
}

void	worker_artifact_destroy(t_worker_artifact *artifact)
{
	if (artifact->executable_path != NULL)
		(void)unlink(artifact->executable_path);
	if (artifact->session_directory != NULL)
		(void)rmdir(artifact->session_directory);
	free(artifact->executable_path);
	free(artifact->session_directory);
	worker_artifact_init(artifact);
}

static char	*worker_root_path(const char *relative_path)
{
	size_t	root_length;
	size_t	relative_length;
	char	*path;

	root_length = strlen(TESTER_SOURCE_ROOT);
	relative_length = strlen(relative_path);
	path = malloc(root_length + relative_length + 2);
	if (path == NULL)
		return (NULL);
	memcpy(path, TESTER_SOURCE_ROOT, root_length);
	path[root_length] = '/';
	memcpy(path + root_length + 1, relative_path, relative_length + 1);
	return (path);
}

static char	*worker_target_path(const char *target, const char *name)
{
	char	*path;
	size_t	length;

	length = strlen(target) + strlen(name) + 2;
	path = malloc(length);
	if (path != NULL)
		(void)snprintf(path, length, "%s/%s", target, name);
	return (path);
}

static int	worker_ensure_directory(const char *path)
{
	if (mkdir(path, 0755) == 0 || errno == EEXIST)
		return (0);
	return (-1);
}

static int	worker_create_session(const char *worker_name,
		t_worker_artifact *artifact)
{
	char	*build_directory;
	char	*sessions_directory;
	char	*template;
	size_t	length;

	build_directory = worker_root_path(".build");
	sessions_directory = worker_root_path(".build/sessions");
	if (build_directory == NULL || sessions_directory == NULL
		|| worker_ensure_directory(build_directory) != 0
		|| worker_ensure_directory(sessions_directory) != 0)
		return (free(build_directory), free(sessions_directory), -1);
	free(build_directory);
	length = strlen(sessions_directory) + strlen("/run.XXXXXX") + 1;
	template = malloc(length);
	if (template == NULL)
		return (free(sessions_directory), -1);
	(void)snprintf(template, length, "%s/run.XXXXXX", sessions_directory);
	free(sessions_directory);
	if (mkdtemp(template) == NULL)
		return (free(template), -1);
	artifact->session_directory = template;
	length = strlen(template) + strlen(worker_name) + 2;
	artifact->executable_path = malloc(length);
	if (artifact->executable_path == NULL)
		return (-1);
	(void)snprintf(artifact->executable_path, length, "%s/%s", template,
		worker_name);
	return (0);
}

static char	*worker_include_argument(const char *relative_path)
{
	char	*path;
	char	*argument;
	size_t	length;

	path = worker_root_path(relative_path);
	if (path == NULL)
		return (NULL);
	length = strlen(path) + 3;
	argument = malloc(length);
	if (argument != NULL)
		(void)snprintf(argument, length, "-I%s", path);
	free(path);
	return (argument);
}

static void	worker_free_strings(char **strings, size_t count)
{
	size_t	index;

	if (strings == NULL)
		return ;
	index = 0;
	while (index < count)
		free(strings[index++]);
	free(strings);
}

static char	**worker_resolve_paths(const char *const paths[], size_t count,
		bool includes)
{
	char	**resolved;
	size_t	index;

	resolved = calloc(count, sizeof(*resolved));
	if (resolved == NULL)
		return (NULL);
	index = 0;
	while (index < count)
	{
		if (includes)
			resolved[index] = worker_include_argument(paths[index]);
		else
			resolved[index] = worker_root_path(paths[index]);
		if (resolved[index] == NULL)
			return (worker_free_strings(resolved, count), NULL);
		index++;
	}
	return (resolved);
}

static void	worker_take_process(t_check_result *check,
		t_process_result *process)
{
	check->exit_code = process->exit_code;
	check->signal_number = process->signal_number;
	check->timed_out = process->timed_out;
	check->duration_ns = process->duration_ns;
	check->stdout_data = process->stdout_data;
	check->stdout_length = process->stdout_length;
	check->stdout_truncated = process->stdout_truncated;
	check->stderr_data = process->stderr_data;
	check->stderr_length = process->stderr_length;
	check->stderr_truncated = process->stderr_truncated;
	process->stdout_data = NULL;
	process->stderr_data = NULL;
}

static char	**worker_compile_arguments(const t_worker_build_plan *plan,
		char *compiler, char **sources, char **includes, char *target_artifact,
		const t_worker_artifact *artifact)
{
	char	**arguments;
	size_t	count;
	size_t	index;
	size_t	item;

	count = 11 + plan->source_count + plan->include_count;
	arguments = calloc(count, sizeof(*arguments));
	if (arguments == NULL)
		return (NULL);
	index = 0;
	arguments[index++] = compiler;
	arguments[index++] = "-std=c11";
	arguments[index++] = "-D_POSIX_C_SOURCE=200809L";
	arguments[index++] = "-D_XOPEN_SOURCE=700";
	arguments[index++] = "-Wall";
	arguments[index++] = "-Wextra";
	arguments[index++] = "-Werror";
	item = 0;
	while (item < plan->include_count)
		arguments[index++] = includes[item++];
	item = 0;
	while (item < plan->source_count)
		arguments[index++] = sources[item++];
	arguments[index++] = target_artifact;
	arguments[index++] = "-o";
	arguments[index++] = artifact->executable_path;
	arguments[index] = NULL;
	return (arguments);
}

static int	worker_run_compiler(const t_worker_build_plan *plan,
		const char *target_path, const t_worker_artifact *artifact,
		t_check_result *check)
{
	char			*compiler;
	char			**sources;
	char			**includes;
	char			*target_artifact;
	char			**arguments;
	t_process_result	process;
	int				status;

	compiler = path_find_executable(getenv("CC") != NULL ? getenv("CC") : "cc");
	sources = worker_resolve_paths(plan->source_paths, plan->source_count, false);
	includes = worker_resolve_paths(plan->include_paths, plan->include_count, true);
	target_artifact = worker_target_path(target_path,
			plan->target_artifact_name);
	arguments = NULL;
	if (compiler != NULL && sources != NULL && includes != NULL
		&& target_artifact != NULL)
		arguments = worker_compile_arguments(plan, compiler, sources, includes,
				target_artifact, artifact);
	if (arguments == NULL)
		return (free(compiler), worker_free_strings(sources, plan->source_count),
			worker_free_strings(includes, plan->include_count),
			free(target_artifact), -1);
	process_result_init(&process);
	status = process_run_capture((const char *const *)arguments,
			TESTER_BUILD_TIMEOUT_MS, TESTER_CAPTURE_LIMIT, &process);
	if (status == 0)
		worker_take_process(check, &process);
	if (status == 0 && process.exited && process.exit_code == 0
		&& access(artifact->executable_path, X_OK) == 0)
		check->status = CHECK_PASS;
	else
	{
		check->status = CHECK_FAIL;
		if (status != 0)
			(void)check_result_set_reason(check,
				"tester failed while invoking the worker compiler");
		else
			(void)check_result_set_reason(check,
				"failed to link the project test worker");
	}
	process_result_destroy(&process);
	free(arguments);
	free(compiler);
	worker_free_strings(sources, plan->source_count);
	worker_free_strings(includes, plan->include_count);
	free(target_artifact);
	return (status);
}

int	worker_build_from_plan(const char *target_path,
		const t_worker_build_plan *plan, t_worker_artifact *artifact,
		t_check_result *result)
{
	int	status;

	check_result_init(result, "Harness");
	worker_artifact_init(artifact);
	if (result->name == NULL || worker_create_session(plan->worker_name,
			artifact) != 0)
	{
		result->status = CHECK_UNKNOWN;
		(void)check_result_set_reason(result,
			"could not create a temporary worker directory");
		return (-1);
	}
	status = worker_run_compiler(plan, target_path, artifact, result);
	if (status != 0 && result->reason == NULL)
		(void)check_result_set_reason(result,
			"could not prepare the project test worker");
	return (status);
}

