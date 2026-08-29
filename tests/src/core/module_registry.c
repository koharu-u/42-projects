#include "libft_module.h"
#include "module.h"
#include <string.h>

const t_project_module	*module_select(const char *requested_name,
		const char *target_path)
{
	if (requested_name != NULL)
	{
		if (strcmp(requested_name, g_libft_module.name) == 0)
			return (&g_libft_module);
		return (NULL);
	}
	if (g_libft_module.probe(target_path))
		return (&g_libft_module);
	return (NULL);
}

