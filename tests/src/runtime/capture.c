#include "capture_internal.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void	capture_buffer_init(t_capture_buffer *buffer, size_t limit)
{
	memset(buffer, 0, sizeof(*buffer));
	buffer->limit = limit;
}

static int	capture_buffer_reserve(t_capture_buffer *buffer, size_t needed)
{
	unsigned char	*data;
	size_t		capacity;

	if (buffer->capacity >= needed)
		return (0);
	capacity = buffer->capacity;
	if (capacity == 0)
		capacity = 256;
	while (capacity < needed)
		capacity *= 2;
	if (capacity > buffer->limit + 1)
		capacity = buffer->limit + 1;
	data = realloc(buffer->data, capacity);
	if (data == NULL)
		return (-1);
	buffer->data = data;
	buffer->capacity = capacity;
	return (0);
}

int	capture_buffer_append(t_capture_buffer *buffer,
		const unsigned char *data, size_t length)
{
	size_t	kept;

	kept = length;
	if (kept > buffer->limit - buffer->length)
	{
		kept = buffer->limit - buffer->length;
		buffer->truncated = true;
	}
	if (kept > 0)
	{
		if (capture_buffer_reserve(buffer, buffer->length + kept + 1) != 0)
			return (-1);
		memcpy(buffer->data + buffer->length, data, kept);
		buffer->length += kept;
		buffer->data[buffer->length] = '\0';
	}
	return (0);
}

int	capture_buffer_read_fd(t_capture_buffer *buffer, int fd, bool *eof)
{
	unsigned char	chunk[8192];
	ssize_t		count;

	*eof = false;
	while (true)
	{
		count = read(fd, chunk, sizeof(chunk));
		if (count > 0)
		{
			if (capture_buffer_append(buffer, chunk, (size_t)count) != 0)
				return (-1);
			continue ;
		}
		if (count == 0)
		{
			*eof = true;
			return (0);
		}
		if (errno == EINTR)
			continue ;
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return (0);
		return (-1);
	}
}

unsigned char	*capture_buffer_take(t_capture_buffer *buffer)
{
	unsigned char	*data;

	if (buffer->data == NULL)
	{
		data = calloc(1, 1);
		return (data);
	}
	data = buffer->data;
	buffer->data = NULL;
	buffer->length = 0;
	buffer->capacity = 0;
	return (data);
}

void	capture_buffer_destroy(t_capture_buffer *buffer)
{
	free(buffer->data);
	memset(buffer, 0, sizeof(*buffer));
}

