*This project has been created as part of the 42 curriculum by kpiwan.*

# ft_printf

## Description

ft_printf is a reimplementation of the mandatory part of the C standard
library `printf` function for the 42 Common Core.

The project introduces variadic functions and formatted output by parsing a
format string, retrieving arguments with `stdarg`, converting values to text,
and writing the result to standard output.

Building the project produces the static archive `libftprintf.a`, which
provides the public function:

~~~c
int	ft_printf(const char *format, ...);
~~~

The implementation supports the mandatory conversions required by the project:
`%c`, `%s`, `%p`, `%d`, `%i`, `%u`, `%x`, `%X`, and `%%`.

## Instructions

### Requirements

- A C compiler (`cc`, Clang, or GCC)
- `make`

### Compilation

From the project directory, run:

~~~sh
make
~~~

This compiles the project with `-Wall -Wextra -Werror` and creates
`libftprintf.a`.

| Command | Result |
| --- | --- |
| `make` | Build `libftprintf.a`. |
| `make clean` | Remove object files. |
| `make fclean` | Remove object files and `libftprintf.a`. |
| `make re` | Rebuild the library from scratch. |

### Using the library

Include the public header:

~~~c
#include "ft_printf.h"
~~~

Compile your program with the static archive after the source or object files
that use it:

~~~sh
cc -Wall -Wextra -Werror -I path/to/ft_printf main.c path/to/ft_printf/libftprintf.a -o program
~~~

Example:

~~~c
#include "ft_printf.h"

int	main(void)
{
	int	printed;

	printed = ft_printf("Hello %s, the answer is %d.\n", "42", 42);
	ft_printf("Printed characters: %d\n", printed);
	return (0);
}
~~~

## Supported conversions

| Conversion | Argument | Output |
| --- | --- | --- |
| `%c` | `int` | Print one character. |
| `%s` | `char *` | Print a NUL-terminated string. |
| `%p` | `void *` | Print a pointer address in hexadecimal form. |
| `%d` | `int` | Print a signed decimal integer. |
| `%i` | `int` | Print a signed decimal integer. |
| `%u` | `unsigned int` | Print an unsigned decimal integer. |
| `%x` | `unsigned int` | Print lowercase hexadecimal. |
| `%X` | `unsigned int` | Print uppercase hexadecimal. |
| `%%` | none | Print a percent sign. |

For a null string pointer, the implementation prints `(null)`. A null pointer
passed to `%p` is printed as `(nil)`, matching the behavior used by the
target Linux environment.

The return value is the number of characters written. Output errors are
propagated as `-1`.

## Implementation overview

The formatter scans the format string from left to right. Ordinary characters
are written directly, while a percent sign introduces a conversion.

`ft_checker` identifies the conversion type. Character and string-like
conversions are handled separately from numeric conversions, and arguments are
retrieved from a `va_list` with the correct promoted type.

Signed and unsigned decimal values are converted before being written.
Hexadecimal and pointer values are emitted recursively using a selected digit
base. Pointer addresses are stored in `uintptr_t` so the integer type is wide
enough to represent a pointer value.

The low-level output helpers use `write`. String output continues writing
until the complete string has been emitted or an output error occurs.

### Scope

This implementation targets the mandatory part of the project only. It does
not implement field width, precision, or the bonus flags `-`, `0`, `.`, `#`,
`+`, and space.

## Resources

- [printf(3) — Linux man-pages](https://man7.org/linux/man-pages/man3/printf.3.html)
- [stdarg(3) — Linux man-pages](https://man7.org/linux/man-pages/man3/stdarg.3.html)
- [write(2) — Linux man-pages](https://man7.org/linux/man-pages/man2/write.2.html)
- [stdint.h integer types — cppreference](https://en.cppreference.com/w/c/types/integer.html)

### AI usage

AI was used to draft this README: organizing the documentation, describing the
supported conversions, and explaining the build and usage workflow. It was
also used as a debugging and review aid to discuss variadic arguments, return
values, write-error propagation, integer edge cases, and tester results.

The implementation itself was developed and tested as part of the learning
process, and the README was checked against the current `Makefile`,
`ft_printf.h`, and project source files.
