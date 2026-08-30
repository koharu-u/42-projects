#include "libft_api.h"
#include "registry.h"
#include "test_case.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int	check_strlen(t_test_context *context, const char *label,
		const char *input)
{
	size_t	expected;
	size_t	actual;

	expected = strlen(input);
	actual = ft_strlen(input);
	if (actual != expected)
		return (test_context_fail(context,
				"input=%s expected=%zu actual=%zu", label, expected, actual));
	return (TEST_CALLBACK_PASS);
}

int	libft_test_strlen_basic(t_test_context *context)
{
	static const char	*const inputs[] = {
		"", "a", "hello", "42", "hello world"
	};
	size_t			index;

	index = 0;
	while (index < sizeof(inputs) / sizeof(inputs[0]))
	{
		if (check_strlen(context, inputs[index], inputs[index]) != 0)
			return (TEST_CALLBACK_FAIL);
		index++;
	}
	return (TEST_CALLBACK_PASS);
}

int	libft_test_strlen_empty(t_test_context *context)
{
	return (check_strlen(context, "\"\"", ""));
}

static int	check_boundary_length(t_test_context *context, size_t length)
{
	char	*input;
	int	status;

	input = malloc(length + 1);
	if (input == NULL)
	{
		(void)test_context_fail(context,
			"tester could not allocate boundary input of length %zu", length);
		return (2);
	}
	memset(input, 'A', length);
	input[length] = '\0';
	status = check_strlen(context, "boundary string", input);
	if (status != 0)
		(void)test_context_fail(context,
			"boundary length=%zu expected=%zu actual differs", length, length);
	free(input);
	return (status);
}

int	libft_test_strlen_boundary(t_test_context *context)
{
	static const size_t	lengths[] = {
		0, 1, 2, 255, 256, 257, 4095, 4096, 4097
	};
	size_t			index;
	int				status;

	index = 0;
	while (index < sizeof(lengths) / sizeof(lengths[0]))
	{
		status = check_boundary_length(context, lengths[index]);
		if (status != TEST_CALLBACK_PASS)
			return (status);
		index++;
	}
	return (TEST_CALLBACK_PASS);
}

static size_t	random_length(t_rng *rng)
{
	static const size_t	special[] = {
		0, 1, 2, 255, 256, 257, 4095, 4096, 4097
	};

	if (rng_range(rng, 100) < 40)
		return (special[rng_range(rng, sizeof(special) / sizeof(special[0]))]);
	return ((size_t)rng_range(rng, 8193));
}

static void	describe_random_input(const unsigned char *input, size_t length,
		char *description, size_t capacity)
{
	size_t	index;
	size_t	offset;
	int		written;

	offset = 0;
	index = 0;
	while (index < length && index < 16 && offset < capacity)
	{
		written = snprintf(description + offset, capacity - offset,
				"%02X%s", input[index], (index + 1 < length && index < 15)
				? " " : "");
		if (written < 0 || (size_t)written >= capacity - offset)
			break ;
		offset += (size_t)written;
		index++;
	}
	if (length == 0)
		(void)snprintf(description, capacity, "<empty>");
	else if (length > 16 && offset < capacity)
		(void)snprintf(description + offset, capacity - offset, " ...");
}

int	libft_test_strlen_random(t_test_context *context)
{
	unsigned char	*input;
	size_t		length;
	size_t		index;
	size_t		expected;
	size_t		actual;
	char		preview[64];

	length = random_length(&context->rng);
	input = malloc(length + 1);
	if (input == NULL)
	{
		(void)test_context_fail(context,
			"tester could not allocate random input of length %zu", length);
		return (2);
	}
	index = 0;
	while (index < length)
		input[index++] = (unsigned char)(rng_range(&context->rng, 255) + 1);
	input[length] = '\0';
	describe_random_input(input, length, preview, sizeof(preview));
	if (test_context_describe_input(context, "length=%zu bytes=[%s]",
			length, preview) != 0)
	{
		free(input);
		(void)test_context_fail(context,
			"tester could not publish the generated input description");
		return (2);
	}
	expected = strlen((const char *)input);
	actual = ft_strlen((const char *)input);
	if (actual != expected)
	{
		free(input);
		return (test_context_fail(context,
				"expected=%zu actual=%zu", expected, actual));
	}
	free(input);
	return (TEST_CALLBACK_PASS);
}

int	libft_register_ft_strlen_tests(t_test_registry *registry)
{
	static const t_test_case	tests[] = {
	{"ft_strlen.basic", "ft_strlen", TEST_BASIC, 1000, 1,
		libft_test_strlen_basic},
	{"ft_strlen.empty", "ft_strlen", TEST_EDGE, 1000, 1,
		libft_test_strlen_empty},
	{"ft_strlen.boundary", "ft_strlen", TEST_BOUNDARY, 1000, 1,
		libft_test_strlen_boundary},
	{"ft_strlen.random", "ft_strlen", TEST_RANDOM, 1000, 100,
		libft_test_strlen_random}
	};
	size_t	index;

	index = 0;
	while (index < sizeof(tests) / sizeof(tests[0]))
	{
		if (registry_add(registry, &tests[index]) != 0)
			return (-1);
		index++;
	}
	return (0);
}
