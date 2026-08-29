#include "process_internal.h"
#include "tester.h"
#include "wire_internal.h"
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct s_event_wire_header
{
	uint32_t	magic;
	uint32_t	version;
	uint32_t	type;
	uint32_t	status;
	uint32_t	test_type;
	uint32_t	flags;
	int32_t		exit_code;
	int32_t		signal_number;
	uint64_t	global_seed;
	uint64_t	test_seed;
	uint64_t	iteration;
	uint64_t	duration_ns;
	uint64_t	passed;
	uint64_t	failed;
	uint64_t	crashed;
	uint64_t	timed_out;
	uint64_t	leaked;
	uint64_t	skipped;
	uint64_t	errors;
	uint32_t	id_length;
	uint32_t	diagnostic_length;
	uint32_t	input_length;
	uint32_t	stdout_length;
	uint32_t	stderr_length;
	uint32_t	message_length;
}   t_event_wire_header;

static void	event_wire_header_init(t_event_wire_header *header,
		t_event_type type)
{
	memset(header, 0, sizeof(*header));
	header->magic = TESTER_WIRE_MAGIC;
	header->version = TESTER_WIRE_VERSION;
	header->type = (uint32_t)type;
}

static int	event_wire_write_header(int fd, const t_event_wire_header *header)
{
	return (process_write_all(fd, header, sizeof(*header)));
}

int	event_wire_send_start(int fd, const t_test_invocation *invocation)
{
	t_event_wire_header	header;
	size_t			length;

	length = strlen(invocation->test->id);
	if (length > 1024)
		return (-1);
	event_wire_header_init(&header, EVENT_TEST_START);
	header.test_type = (uint32_t)invocation->test->type;
	header.global_seed = invocation->global_seed;
	header.test_seed = invocation->test_seed;
	header.iteration = invocation->iteration;
	header.id_length = (uint32_t)length;
	if (event_wire_write_header(fd, &header) != 0)
		return (-1);
	return (process_write_all(fd, invocation->test->id, length));
}

int	event_wire_send_result(int fd, const t_test_result *result)
{
	t_event_wire_header	header;
	size_t			id_length;
	size_t			diagnostic_length;
	size_t			input_length;

	id_length = strlen(result->test_id);
	diagnostic_length = strlen(result->diagnostic);
	input_length = 0;
	if (result->input_description != NULL)
		input_length = strlen(result->input_description);
	if (id_length > 1024 || diagnostic_length >= TESTER_DIAGNOSTIC_CAP
		|| input_length >= TESTER_INPUT_DESCRIPTION_CAP
		|| result->stdout_length > TESTER_CAPTURE_LIMIT
		|| result->stderr_length > TESTER_CAPTURE_LIMIT)
		return (-1);
	event_wire_header_init(&header, EVENT_TEST_RESULT);
	header.status = (uint32_t)result->status;
	header.flags = (uint32_t)result->stdout_truncated
		| ((uint32_t)result->stderr_truncated << 1U);
	header.exit_code = result->exit_code;
	header.signal_number = result->signal_number;
	header.global_seed = result->global_seed;
	header.test_seed = result->test_seed;
	header.iteration = result->iteration;
	header.duration_ns = result->duration_ns;
	header.id_length = (uint32_t)id_length;
	header.diagnostic_length = (uint32_t)diagnostic_length;
	header.input_length = (uint32_t)input_length;
	header.stdout_length = (uint32_t)result->stdout_length;
	header.stderr_length = (uint32_t)result->stderr_length;
	if (event_wire_write_header(fd, &header) != 0
		|| process_write_all(fd, result->test_id, id_length) != 0
		|| process_write_all(fd, result->diagnostic,
			diagnostic_length) != 0
		|| process_write_all(fd, result->input_description,
			input_length) != 0
		|| process_write_all(fd, result->stdout_data,
			result->stdout_length) != 0
		|| process_write_all(fd, result->stderr_data,
			result->stderr_length) != 0)
		return (-1);
	return (0);
}

int	event_wire_send_finished(int fd, const t_run_summary *summary)
{
	t_event_wire_header	header;

	event_wire_header_init(&header, EVENT_FINISHED);
	header.passed = summary->passed;
	header.failed = summary->failed;
	header.crashed = summary->crashed;
	header.timed_out = summary->timed_out;
	header.leaked = summary->leaked;
	header.skipped = summary->skipped;
	header.errors = summary->errors;
	return (event_wire_write_header(fd, &header));
}

int	event_wire_send_error(int fd, const char *message)
{
	t_event_wire_header	header;
	size_t			length;

	length = strlen(message);
	if (length > 4096)
		length = 4096;
	event_wire_header_init(&header, EVENT_ERROR);
	header.message_length = (uint32_t)length;
	if (event_wire_write_header(fd, &header) != 0)
		return (-1);
	return (process_write_all(fd, message, length));
}

static int	event_wire_read_header(int fd, t_event_wire_header *header)
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
			return (remaining == sizeof(*header));
		if (errno == EINTR)
			continue ;
		return (-1);
	}
	return (0);
}

static void	*event_wire_read_payload(int fd, uint32_t length, bool string)
{
	unsigned char	*data;
	bool			clean_eof;

	data = calloc((size_t)length + (size_t)string, 1);
	if (data == NULL)
		return (NULL);
	if (length > 0 && process_read_all(fd, data, length, &clean_eof) != 0)
	{
		free(data);
		return (NULL);
	}
	return (data);
}

static bool	event_wire_header_valid(const t_event_wire_header *header)
{
	if (header->magic != TESTER_WIRE_MAGIC
		|| header->version != TESTER_WIRE_VERSION
		|| header->type > (uint32_t)EVENT_ERROR
		|| header->id_length > 1024
		|| header->diagnostic_length >= TESTER_DIAGNOSTIC_CAP
		|| header->input_length >= TESTER_INPUT_DESCRIPTION_CAP
		|| header->stdout_length > TESTER_CAPTURE_LIMIT
		|| header->stderr_length > TESTER_CAPTURE_LIMIT
		|| header->message_length > 4096)
		return (false);
	if (header->type == (uint32_t)EVENT_TEST_RESULT
		&& header->status > (uint32_t)TEST_ERROR)
		return (false);
	if (header->type == (uint32_t)EVENT_TEST_START
		&& header->test_type > (uint32_t)TEST_MEMORY)
		return (false);
	return (true);
}

static int	event_wire_receive_start(int fd,
		const t_event_wire_header *header, const t_event_sink *sink)
{
	char			*id;
	t_test_case		test;
	t_test_invocation	invocation;
	t_event			event;

	id = event_wire_read_payload(fd, header->id_length, true);
	if (id == NULL)
		return (-1);
	memset(&test, 0, sizeof(test));
	memset(&invocation, 0, sizeof(invocation));
	memset(&event, 0, sizeof(event));
	test.id = id;
	test.type = (t_test_type)header->test_type;
	invocation.test = &test;
	invocation.global_seed = header->global_seed;
	invocation.test_seed = header->test_seed;
	invocation.iteration = header->iteration;
	event.type = EVENT_TEST_START;
	event.invocation = &invocation;
	event_emit(sink, &event);
	free(id);
	return (0);
}

static int	event_wire_receive_result(int fd,
		const t_event_wire_header *header, const t_event_sink *sink)
{
	t_test_result	result;
	t_event		event;

	test_result_init(&result);
	result.test_id = event_wire_read_payload(fd, header->id_length, true);
	result.diagnostic = event_wire_read_payload(fd,
			header->diagnostic_length, true);
	result.input_description = event_wire_read_payload(fd,
			header->input_length, true);
	result.stdout_data = event_wire_read_payload(fd,
			header->stdout_length, true);
	result.stderr_data = event_wire_read_payload(fd,
			header->stderr_length, true);
	if (result.test_id == NULL || result.diagnostic == NULL
		|| result.input_description == NULL
		|| result.stdout_data == NULL || result.stderr_data == NULL)
		return (test_result_destroy(&result), -1);
	result.status = (t_test_status)header->status;
	result.exit_code = header->exit_code;
	result.signal_number = header->signal_number;
	result.global_seed = header->global_seed;
	result.test_seed = header->test_seed;
	result.iteration = header->iteration;
	result.duration_ns = header->duration_ns;
	result.stdout_length = header->stdout_length;
	result.stderr_length = header->stderr_length;
	result.stdout_truncated = (header->flags & 1U) != 0;
	result.stderr_truncated = (header->flags & 2U) != 0;
	memset(&event, 0, sizeof(event));
	event.type = EVENT_TEST_RESULT;
	event.result = &result;
	event_emit(sink, &event);
	test_result_destroy(&result);
	return (0);
}

static void	event_wire_receive_finished(const t_event_wire_header *header,
		const t_event_sink *sink)
{
	t_run_summary	summary;
	t_event		event;

	summary.passed = header->passed;
	summary.failed = header->failed;
	summary.crashed = header->crashed;
	summary.timed_out = header->timed_out;
	summary.leaked = header->leaked;
	summary.skipped = header->skipped;
	summary.errors = header->errors;
	memset(&event, 0, sizeof(event));
	event.type = EVENT_FINISHED;
	event.summary = &summary;
	event_emit(sink, &event);
}

static int	event_wire_receive_error(int fd,
		const t_event_wire_header *header, const t_event_sink *sink)
{
	char	*message;
	t_event	event;

	message = event_wire_read_payload(fd, header->message_length, true);
	if (message == NULL)
		return (-1);
	memset(&event, 0, sizeof(event));
	event.type = EVENT_ERROR;
	event.message = message;
	event_emit(sink, &event);
	free(message);
	return (0);
}

int	event_wire_receive_all(int fd, const t_event_sink *sink)
{
	t_event_wire_header	header;
	int				status;

	while (true)
	{
		status = event_wire_read_header(fd, &header);
		if (status == 1)
			return (0);
		if (status != 0 || !event_wire_header_valid(&header))
			return (-1);
		if (header.type == (uint32_t)EVENT_TEST_START)
			status = event_wire_receive_start(fd, &header, sink);
		else if (header.type == (uint32_t)EVENT_TEST_RESULT)
			status = event_wire_receive_result(fd, &header, sink);
		else if (header.type == (uint32_t)EVENT_FINISHED)
		{
			event_wire_receive_finished(&header, sink);
			status = 0;
		}
		else if (header.type == (uint32_t)EVENT_ERROR)
			status = event_wire_receive_error(fd, &header, sink);
		else
			return (-1);
		if (status != 0)
			return (-1);
	}
}
