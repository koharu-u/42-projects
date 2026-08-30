#include "libft_module.h"
#include "process_internal.h"
#include "tester.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static char	*libft_join_path(const char *directory, const char *name)
{
	size_t	directory_length;
	size_t	name_length;
	char	*path;

	directory_length = strlen(directory);
	name_length = strlen(name);
	path = malloc(directory_length + name_length + 2);
	if (path == NULL)
		return (NULL);
	memcpy(path, directory, directory_length);
	path[directory_length] = '/';
	memcpy(path + directory_length + 1, name, name_length + 1);
	return (path);
}

static bool	libft_path_is_file(const char *path)
{
	struct stat	information;

	return (stat(path, &information) == 0 && S_ISREG(information.st_mode));
}

static bool	libft_probe(const char *target_path)
{
	char	*source;
	char	*archive;
	const char	*name;
	size_t	index;
	bool	found;

	source = libft_join_path(target_path, "ft_strlen.c");
	archive = libft_join_path(target_path, "libft.a");
	if (source == NULL || archive == NULL)
		found = false;
	else
		found = libft_path_is_file(source) || libft_path_is_file(archive);
	free(source);
	free(archive);
	index = 0;
	while (!found && index < libft_function_count())
	{
		name = libft_function_source_name(libft_function_name_at(index++));
		source = libft_join_path(target_path, name);
		if (source != NULL)
			found = libft_path_is_file(source);
		free(source);
	}
	return (found);
}

static int	append_check_bytes(unsigned char **destination,
		size_t *destination_length, const unsigned char *source,
		size_t source_length, bool *truncated)
{
	unsigned char	*data;
	size_t		available;
	size_t		kept;

	available = TESTER_CAPTURE_LIMIT - *destination_length;
	kept = source_length;
	if (kept > available)
	{
		kept = available;
		*truncated = true;
	}
	data = realloc(*destination, *destination_length + kept + 1);
	if (data == NULL)
		return (-1);
	*destination = data;
	if (kept > 0)
		memcpy(data + *destination_length, source, kept);
	*destination_length += kept;
	data[*destination_length] = '\0';
	return (0);
}

static int	libft_take_build_output(t_check_result *check,
		const t_process_result *process)
{
	if (append_check_bytes(&check->stdout_data, &check->stdout_length,
			process->stdout_data, process->stdout_length,
			&check->stdout_truncated) != 0
		|| append_check_bytes(&check->stderr_data, &check->stderr_length,
			process->stderr_data, process->stderr_length,
			&check->stderr_truncated) != 0)
		return (-1);
	check->stdout_truncated = check->stdout_truncated
		|| process->stdout_truncated;
	check->stderr_truncated = check->stderr_truncated
		|| process->stderr_truncated;
	check->duration_ns += process->duration_ns;
	check->exit_code = process->exit_code;
	check->signal_number = process->signal_number;
	check->timed_out = process->timed_out;
	return (0);
}

static int	libft_run_make(const char *make_path, const char *target_path,
		bool clean, t_check_result *check)
{
	const char		*arguments[6];
	t_process_result	process;
	int				status;

	arguments[0] = make_path;
	arguments[1] = "-C";
	arguments[2] = target_path;
	if (clean)
		arguments[3] = "clean";
	else
		arguments[3] = NULL;
	arguments[4] = NULL;
	arguments[5] = NULL;
	process_result_init(&process);
	status = process_run_capture(arguments, TESTER_BUILD_TIMEOUT_MS,
			TESTER_CAPTURE_LIMIT, &process);
	if (status == 0)
		status = libft_take_build_output(check, &process);
	if (status == 0 && (process.timed_out || process.signaled
			|| !process.exited || process.exit_code != 0))
		status = 1;
	process_result_destroy(&process);
	return (status);
}

static int	libft_build(const char *target_path, t_check_result *result)
{
	char	*make_path;
	char	*archive_path;
	int	status;

	check_result_init(result, "Build");
	if (result->name == NULL)
		return (-1);
	make_path = path_find_executable("make");
	if (make_path == NULL)
	{
		result->status = CHECK_UNKNOWN;
		return (check_result_set_reason(result,
				"make executable not found"));
	}
	status = libft_run_make(make_path, target_path, true, result);
	if (status == 0)
		status = libft_run_make(make_path, target_path, false, result);
	free(make_path);
	if (status < 0)
	{
		result->status = CHECK_UNKNOWN;
		return (check_result_set_reason(result,
				"tester failed while running make"));
	}
	if (status > 0)
	{
		result->status = CHECK_FAIL;
		return (check_result_set_reason(result,
				"clean build did not complete successfully"));
	}
	archive_path = libft_join_path(target_path, "libft.a");
	if (archive_path == NULL)
		return (-1);
	if (!libft_path_is_file(archive_path))
	{
		free(archive_path);
		result->status = CHECK_FAIL;
		return (check_result_set_reason(result,
				"clean build succeeded but libft.a was not produced"));
	}
	free(archive_path);
	result->status = CHECK_PASS;
	return (0);
}

const t_project_module	g_libft_module = {
	.name = "libft",
	.probe = libft_probe,
	.build = libft_build,
	.build_worker = libft_build_worker,
	.function_count = libft_function_count,
	.function_name_at = libft_function_name_at,
	.function_source_name = libft_function_source_name,
	.function_has_tests = libft_function_has_tests
};
