#include "process_internal.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static bool	is_executable_file(const char *path)
{
	struct stat	information;

	return (access(path, X_OK) == 0 && stat(path, &information) == 0
		&& S_ISREG(information.st_mode));
}

static char	*join_path_component(const char *component, size_t length,
		const char *name)
{
	char	*path;
	size_t	name_length;

	if (length == 0)
	{
		component = ".";
		length = 1;
	}
	name_length = strlen(name);
	path = malloc(length + name_length + 2);
	if (path == NULL)
		return (NULL);
	memcpy(path, component, length);
	path[length] = '/';
	memcpy(path + length + 1, name, name_length + 1);
	return (path);
}

char	*path_find_executable(const char *name)
{
	const char	*path_value;
	const char	*component;
	const char	*separator;
	char			*candidate;

	if (strchr(name, '/') != NULL)
	{
		if (is_executable_file(name))
			return (strdup(name));
		return (NULL);
	}
	path_value = getenv("PATH");
	if (path_value == NULL)
		return (NULL);
	component = path_value;
	while (true)
	{
		separator = strchr(component, ':');
		if (separator == NULL)
			separator = component + strlen(component);
		candidate = join_path_component(component,
				(size_t)(separator - component), name);
		if (candidate == NULL || is_executable_file(candidate))
			return (candidate);
		free(candidate);
		if (*separator == '\0')
			break ;
		component = separator + 1;
	}
	return (NULL);
}
