*This project has been created as part of the 42 curriculum by kpiwan.*

# Libft

## Description

Libft is a personal C library created for the first project of the 42 Common
Core. Its goal is to build a reliable collection of reusable utilities that
can serve as a foundation in later C projects.

The library reimplements commonly used C library functions and provides
additional tools for strings, memory, file-descriptor output, and singly
linked lists. Building the project produces the static archive `libft.a`.

## Instructions

### Requirements

- A C compiler (`cc`, Clang, or GCC)
- `make`

### Compilation

From the project directory, run:

~~~sh
make
~~~

This compiles the library with `-Wall -Wextra -Werror` and creates `libft.a`.

| Command | Result |
| --- | --- |
| `make` | Build `libft.a`. |
| `make clean` | Remove object files. |
| `make fclean` | Remove object files and `libft.a`. |
| `make re` | Rebuild the library from scratch. |

### Using the library

Include the public header:

~~~c
#include "libft.h"
~~~

Compile your program with the Libft include directory and static archive:

~~~sh
cc -Wall -Wextra -Werror -I path/to/libft main.c path/to/libft/libft.a -o program
~~~

Example:

~~~c
#include "libft.h"

int	main(void)
{
	char	*message;

	message = ft_strjoin("Hello, ", "42!\n");
	if (!message)
		return (1);
	ft_putstr_fd(message, 1);
	free(message);
	return (0);
}
~~~

## Library overview

All public declarations and the `t_list` type are available in
[`libft.h`](libft.h). Functions with a standard-library counterpart use the
same intended behavior with an `ft_` prefix.

| Category | Functions | Description |
| --- | --- | --- |
| Character checks and conversion | `ft_isalpha`, `ft_isdigit`, `ft_isalnum`, `ft_isascii`, `ft_isprint`, `ft_toupper`, `ft_tolower` | Test character classes and convert ASCII letter case. |
| Memory management and operations | `ft_memset`, `ft_bzero`, `ft_memcpy`, `ft_memmove`, `ft_memchr`, `ft_memcmp`, `ft_calloc` | Initialize, copy, search, compare, and allocate memory. |
| String inspection and comparison | `ft_strlen`, `ft_strlcpy`, `ft_strlcat`, `ft_strchr`, `ft_strrchr`, `ft_strncmp`, `ft_strnstr` | Measure, copy, append, locate, and compare NUL-terminated strings. |
| Conversion and duplication | `ft_atoi`, `ft_itoa`, `ft_strdup` | Convert integers and strings, or create a duplicate string. |
| Allocated string transformations | `ft_substr`, `ft_strjoin`, `ft_strtrim`, `ft_split`, `ft_strmapi`, `ft_striteri` | Create, combine, trim, split, and transform strings. |
| File-descriptor output | `ft_putchar_fd`, `ft_putstr_fd`, `ft_putendl_fd`, `ft_putnbr_fd` | Write characters, strings, lines, and integers to a given file descriptor. |
| Singly linked lists | `ft_lstnew`, `ft_lstadd_front`, `ft_lstadd_back`, `ft_lstsize`, `ft_lstlast`, `ft_lstdelone`, `ft_lstclear`, `ft_lstiter`, `ft_lstmap` | Create, modify, traverse, transform, and destroy `t_list` nodes. |

Functions that allocate memory return `NULL` if allocation fails. The caller
owns successful allocations and must release them with `free`. List-destruction
functions accept a callback so the caller can also free each node's content.

## Resources

- [Linux man-pages](https://man7.org/linux/man-pages/)
- [geeksforgeeks](https://www.geeksforgeeks.org/)

### AI usage

AI was used to draft this README: organizing its sections, writing the build
and usage documentation, grouping the APIs, and suggesting references. It was
also used as a debugging aid to discuss unexpected cases, review potential
edge cases, and identify common implementation mistakes. The README content
was checked against the project's `Makefile` and `libft.h`.
