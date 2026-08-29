#include "libft_module.h"

int	libft_build_worker(const char *target_path, t_worker_artifact *artifact,
		t_check_result *result)
{
	static const char *const	sources[] = {
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
		"modules/libft/worker.c",
		"modules/libft/part1/ft_strlen.c"
	};
	static const char *const	includes[] = {
		"include",
		"src/internal",
		"modules/libft"
	};
	static const t_worker_build_plan	plan = {
		.worker_name = "libft-worker",
		.source_paths = sources,
		.source_count = sizeof(sources) / sizeof(sources[0]),
		.include_paths = includes,
		.include_count = sizeof(includes) / sizeof(includes[0]),
		.target_artifact_name = "libft.a"
	};

	return (worker_build_from_plan(target_path, &plan, artifact, result));
}

