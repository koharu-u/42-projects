#include "libft_module.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static const char *const	g_worker_sources[] = {
	"src/core/result.c",
	"src/core/registry.c",
	"src/core/scheduler.c",
	"src/core/event.c",
	"src/fuzz/rng.c",
	"src/runtime/timeout.c",
	"src/runtime/capture.c",
	"src/runtime/process.c",
	"src/runtime/signal.c",
	"src/runtime/result_wire.c",
	"src/runtime/runner.c",
	"src/runtime/event_wire.c",
	"src/runtime/worker_main.c",
	"modules/libft/worker.c"
};

static const char *const	g_worker_includes[] = {
	"include",
	"src/internal",
	"modules/libft"
};

static char	*libft_symbol_define(const char *function_name)
{
	char	*definition;
	size_t	length;

	length = strlen("LIBFT_REQUIRED_SYMBOL=") + strlen(function_name) + 1;
	definition = malloc(length);
	if (definition != NULL)
		(void)snprintf(definition, length, "LIBFT_REQUIRED_SYMBOL=%s",
			function_name);
	return (definition);
}

static const char *const	g_test_sources[] = {
	"modules/libft/part1/ft_strlen.c",
	"modules/libft/part1/ft_isalpha.c",
	"modules/libft/part1/ft_isdigit.c",
	"modules/libft/part1/ft_memset.c",
	"modules/libft/part1/ft_memcpy.c"
};

static const char *const	g_test_defines[] = {
	"LIBFT_INCLUDE_FT_STRLEN=1",
	"LIBFT_INCLUDE_FT_ISALPHA=1",
	"LIBFT_INCLUDE_FT_ISDIGIT=1",
	"LIBFT_INCLUDE_FT_MEMSET=1",
	"LIBFT_INCLUDE_FT_MEMCPY=1"
};

static bool	libft_path_is_file(const char *path)
{
	struct stat	information;

	return (stat(path, &information) == 0 && S_ISREG(information.st_mode));
}

static char	*libft_join_path(const char *directory, const char *name)
{
	char	*path;
	size_t	length;

	length = strlen(directory) + strlen(name) + 2;
	path = malloc(length);
	if (path != NULL)
		(void)snprintf(path, length, "%s/%s", directory, name);
	return (path);
}

static bool	libft_source_in_list(char *const sources[], size_t source_count,
		const char *source)
{
	size_t	index;

	index = 0;
	while (index < source_count)
	{
		if (strcmp(sources[index], source) == 0)
			return (true);
		index++;
	}
	return (false);
}

static int	libft_set_missing(t_check_result *result, t_check_status status,
		const char *prefix, const char *name)
{
	char	*reason;
	size_t	length;

	length = strlen(prefix) + strlen(name) + 1;
	reason = malloc(length);
	if (reason == NULL)
		return (-1);
	(void)snprintf(reason, length, "%s%s", prefix, name);
	result->status = status;
	if (check_result_set_reason(result, reason) != 0)
		return (free(reason), -1);
	free(reason);
	return (0);
}

static int	libft_collect_sources(const char *directory,
		const t_libft_function *function, const char *root_source,
		char *sources[], size_t *source_count, t_check_result *result)
{
	const t_libft_function	*dependency;
	const char *const		*dependencies;
	char				*path;
	size_t				index;

	path = (char *)root_source;
	if (path == NULL)
		path = libft_join_path(directory, function->source_name);
	if (path == NULL)
		return (-1);
	if (!libft_path_is_file(path))
	{
		if (root_source == NULL)
			return (free(path), libft_set_missing(result, CHECK_BLOCKED,
					"missing dependency: ", function->source_name));
		return (libft_set_missing(result, CHECK_MISSING, "source not found: ",
				function->source_name));
	}
	if (!libft_source_in_list(sources, *source_count, path))
	{
		if (*source_count >= 40)
			return (root_source == NULL ? free(path) : (void)0, -1);
		if (root_source == NULL)
			sources[*source_count] = path;
		else
		{
			sources[*source_count] = strdup(path);
			if (sources[*source_count] == NULL)
				return (-1);
		}
		(*source_count)++;
	}
	else if (root_source == NULL)
		free(path);
	dependencies = function->dependencies;
	index = 0;
	while (dependencies != NULL && dependencies[index] != NULL)
	{
		dependency = libft_function_find(dependencies[index]);
		if (dependency == NULL)
			return (-1);
		if (libft_collect_sources(directory, dependency, NULL, sources,
				&*source_count, result) != 0)
			return (-1);
		if (result->status == CHECK_BLOCKED || result->status == CHECK_MISSING)
			return (0);
		index++;
	}
	return (0);
}

static void	libft_free_sources(char *sources[], size_t source_count)
{
	size_t	index;

	index = 0;
	while (index < source_count)
		free(sources[index++]);
}

static int	libft_build_direct(const t_build_request *request,
		const t_libft_function *function, t_build_artifact *artifact,
		t_check_result *result)
{
	const char		*sources[16];
	const char		*defines[2];
	const char		*includes[1];
	char				*target_sources[40];
	char				*symbol_define;
	t_worker_build_plan	plan;
	size_t				source_count;
	size_t				index;
	int					status;

	check_result_init(result, request->check_name);
	if (result->name == NULL)
		return (-1);
	memset(target_sources, 0, sizeof(target_sources));
	source_count = 0;
	status = libft_collect_sources(request->target_path, function,
			request->source_path, target_sources, &source_count, result);
	if (status != 0 || result->status == CHECK_BLOCKED
		|| result->status == CHECK_MISSING)
		return (libft_free_sources(target_sources, source_count), status);
	symbol_define = libft_symbol_define(function->name);
	if (symbol_define == NULL)
		return (libft_free_sources(target_sources, source_count), -1);
	index = 0;
	while (index < sizeof(g_worker_sources) / sizeof(g_worker_sources[0]))
	{
		sources[index] = g_worker_sources[index];
		index++;
	}
	if (function->test_source_path != NULL)
		sources[index++] = function->test_source_path;
	sources[index++] = "modules/libft/symbol_probe.c";
	includes[0] = request->target_path;
	index = 0;
	if (function->test_define != NULL)
		defines[index++] = function->test_define;
	defines[index++] = symbol_define;
	memset(&plan, 0, sizeof(plan));
	plan.worker_name = "libft-direct-worker";
	plan.source_paths = sources;
	plan.source_count = sizeof(g_worker_sources) / sizeof(g_worker_sources[0])
		+ 1 + (function->test_source_path != NULL);
	plan.include_paths = g_worker_includes;
	plan.include_count = sizeof(g_worker_includes) / sizeof(g_worker_includes[0]);
	plan.external_include_paths = includes;
	plan.external_include_count = 1;
	plan.target_source_paths = (const char *const *)target_sources;
	plan.target_source_count = source_count;
	plan.defines = defines;
	plan.define_count = index;
	plan.check_name = request->check_name;
	plan.backend = BUILD_BACKEND_DIRECT;
	plan.keep_temporary_files = request->keep_temporary_files;
	check_result_destroy(result);
	status = worker_build_from_plan(request->target_path, &plan, artifact, result);
	free(symbol_define);
	libft_free_sources(target_sources, source_count);
	return (status);
}

static int	libft_build_make(const t_build_request *request,
		t_build_artifact *artifact, t_check_result *result)
{
	const char		*sources[20];
	t_worker_build_plan	plan;
	size_t				index;

	index = 0;
	while (index < sizeof(g_worker_sources) / sizeof(g_worker_sources[0]))
	{
		sources[index] = g_worker_sources[index];
		index++;
	}
	memcpy(sources + index, g_test_sources, sizeof(g_test_sources));
	index += sizeof(g_test_sources) / sizeof(g_test_sources[0]);
	memset(&plan, 0, sizeof(plan));
	plan.worker_name = "libft-worker";
	plan.source_paths = sources;
	plan.source_count = index;
	plan.include_paths = g_worker_includes;
	plan.include_count = sizeof(g_worker_includes) / sizeof(g_worker_includes[0]);
	plan.defines = g_test_defines;
	plan.define_count = sizeof(g_test_defines) / sizeof(g_test_defines[0]);
	plan.target_artifact_name = "libft.a";
	plan.check_name = "Harness";
	plan.backend = BUILD_BACKEND_MAKE;
	plan.keep_temporary_files = request->keep_temporary_files;
	return (worker_build_from_plan(request->target_path, &plan, artifact, result));
}

int	libft_build_worker(const t_build_request *request,
		t_build_artifact *artifact, t_check_result *result)
{
	const t_libft_function	*function;

	if (request->backend == BUILD_BACKEND_MAKE)
		return (libft_build_make(request, artifact, result));
	function = libft_function_find(request->function_name);
	if (function == NULL)
	{
		check_result_init(result, request->check_name);
		if (result->name != NULL)
		{
			result->status = CHECK_UNKNOWN;
			(void)check_result_set_reason(result, "unknown Libft function");
		}
		return (-1);
	}
	return (libft_build_direct(request, function, artifact, result));
}
