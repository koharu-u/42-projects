#include "registry.h"
#include <stdlib.h>
#include <string.h>

void	registry_init(t_test_registry *registry)
{
	memset(registry, 0, sizeof(*registry));
}

static int	registry_reserve(t_test_registry *registry, size_t needed)
{
	t_test_case	*items;
	size_t		capacity;

	if (registry->capacity >= needed)
		return (0);
	capacity = registry->capacity;
	if (capacity == 0)
		capacity = 8;
	while (capacity < needed)
		capacity *= 2;
	items = realloc(registry->items, capacity * sizeof(*items));
	if (items == NULL)
		return (-1);
	registry->items = items;
	registry->capacity = capacity;
	return (0);
}

int	registry_add(t_test_registry *registry, const t_test_case *test)
{
	if (registry->frozen || test == NULL || test->id == NULL
		|| test->group == NULL || test->run == NULL || test->id[0] == '\0')
		return (-1);
	if (registry_find(registry, test->id) != NULL)
		return (-1);
	if (registry_reserve(registry, registry->count + 1) != 0)
		return (-1);
	registry->items[registry->count] = *test;
	registry->count++;
	return (0);
}

void	registry_freeze(t_test_registry *registry)
{
	registry->frozen = true;
}

const t_test_case	*registry_find(const t_test_registry *registry,
		const char *test_id)
{
	size_t	index;

	index = 0;
	while (index < registry->count)
	{
		if (strcmp(registry->items[index].id, test_id) == 0)
			return (&registry->items[index]);
		index++;
	}
	return (NULL);
}

bool	registry_has_group(const t_test_registry *registry, const char *group)
{
	size_t	index;

	index = 0;
	while (index < registry->count)
	{
		if (strcmp(registry->items[index].group, group) == 0)
			return (true);
		index++;
	}
	return (false);
}

const t_test_case	*registry_at(const t_test_registry *registry, size_t index)
{
	if (index >= registry->count)
		return (NULL);
	return (&registry->items[index]);
}

size_t	registry_count(const t_test_registry *registry)
{
	return (registry->count);
}

void	registry_destroy(t_test_registry *registry)
{
	free(registry->items);
	memset(registry, 0, sizeof(*registry));
}
