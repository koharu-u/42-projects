#ifndef REGISTRY_H
# define REGISTRY_H

# include "test_case.h"
# include <stdbool.h>
# include <stddef.h>

typedef struct s_test_registry
{
	t_test_case	*items;
	size_t		count;
	size_t		capacity;
	bool		frozen;
}   t_test_registry;

void				registry_init(t_test_registry *registry);
int				registry_add(t_test_registry *registry,
						const t_test_case *test);
void				registry_freeze(t_test_registry *registry);
const t_test_case	*registry_find(const t_test_registry *registry,
						const char *test_id);
const t_test_case	*registry_at(const t_test_registry *registry, size_t index);
size_t				registry_count(const t_test_registry *registry);
void				registry_destroy(t_test_registry *registry);

#endif

