#include "process_internal.h"
#include "test_case.h"
#include "tester.h"
#include "wire_internal.h"
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum e_child_frame_type
{
	CHILD_FRAME_INPUT = 1,
	CHILD_FRAME_FINAL = 2
};

typedef struct s_child_wire_header
{
	uint32_t	magic;
	uint32_t	version;
	uint32_t	frame_type;
	uint32_t	status;
	uint32_t	payload_length;
}   t_child_wire_header;

static int	child_wire_send_frame(int fd, uint32_t frame_type,
		t_test_status status, const char *payload, size_t maximum)
{
	t_child_wire_header	header;
	size_t			length;

	length = 0;
	if (payload != NULL)
		length = strlen(payload);
	if (length >= maximum)
		length = maximum - 1;
	header.magic = TESTER_WIRE_MAGIC;
	header.version = TESTER_WIRE_VERSION;
	header.frame_type = frame_type;
	header.status = (uint32_t)status;
	header.payload_length = (uint32_t)length;
	if (process_write_all(fd, &header, sizeof(header)) != 0)
		return (-1);
	if (length > 0 && process_write_all(fd, payload, length) != 0)
		return (-1);
	return (0);
}

int	child_wire_send(int fd, t_test_status status, const char *diagnostic)
{
	return (child_wire_send_frame(fd, CHILD_FRAME_FINAL, status, diagnostic,
			TESTER_DIAGNOSTIC_CAP));
}

int	test_context_describe_input(t_test_context *context,
		const char *format, ...)
{
	char		message[TESTER_INPUT_DESCRIPTION_CAP];
	va_list	arguments;
	int		written;

	va_start(arguments, format);
	written = vsnprintf(message, sizeof(message), format, arguments);
	va_end(arguments);
	if (written < 0)
		return (-1);
	return (child_wire_send_frame(context->_control_fd, CHILD_FRAME_INPUT,
			TEST_PASS, message, TESTER_INPUT_DESCRIPTION_CAP));
}

static int	child_wire_read_header(int fd, t_child_wire_header *header)
{
	unsigned char	*bytes;
	size_t		remaining;
	ssize_t		count;

	bytes = (unsigned char *)header;
	remaining = sizeof(*header);
	while (remaining > 0)
	{
		count = read(fd, bytes, remaining);
		if (count > 0)
		{
			bytes += count;
			remaining -= (size_t)count;
			continue ;
		}
		if (count == 0)
			return (remaining == sizeof(*header) ? 1 : -1);
		if (errno == EINTR)
			continue ;
		return (-1);
	}
	return (0);
}

static char	*child_wire_read_payload(int fd, uint32_t length)
{
	char	*payload;
	bool	clean_eof;

	payload = calloc((size_t)length + 1, 1);
	if (payload == NULL)
		return (NULL);
	if (length > 0 && process_read_all(fd, payload, length, &clean_eof) != 0)
	{
		free(payload);
		return (NULL);
	}
	return (payload);
}

static bool	child_wire_header_valid(const t_child_wire_header *header)
{
	if (header->magic != TESTER_WIRE_MAGIC
		|| header->version != TESTER_WIRE_VERSION
		|| (header->frame_type != CHILD_FRAME_INPUT
			&& header->frame_type != CHILD_FRAME_FINAL))
		return (false);
	if (header->frame_type == CHILD_FRAME_INPUT)
		return (header->payload_length < TESTER_INPUT_DESCRIPTION_CAP);
	return (header->status <= (uint32_t)TEST_ERROR
		&& header->payload_length < TESTER_DIAGNOSTIC_CAP);
}

int	child_wire_receive(int fd, t_test_status *status, char **diagnostic,
		char **input_description)
{
	t_child_wire_header	header;
	char			*payload;
	int				read_status;
	bool			final_received;

	*diagnostic = strdup("");
	*input_description = strdup("");
	if (*diagnostic == NULL || *input_description == NULL)
	{
		free(*diagnostic);
		free(*input_description);
		*diagnostic = NULL;
		*input_description = NULL;
		return (-1);
	}
	final_received = false;
	while (true)
	{
		read_status = child_wire_read_header(fd, &header);
		if (read_status == 1)
			break ;
		if (read_status != 0 || !child_wire_header_valid(&header))
			return (-1);
		payload = child_wire_read_payload(fd, header.payload_length);
		if (payload == NULL)
			return (-1);
		if (header.frame_type == CHILD_FRAME_INPUT)
		{
			free(*input_description);
			*input_description = payload;
		}
		else
		{
			free(*diagnostic);
			*diagnostic = payload;
			*status = (t_test_status)header.status;
			final_received = true;
		}
	}
	return (final_received ? 0 : 1);
}
