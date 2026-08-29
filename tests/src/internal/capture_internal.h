#ifndef CAPTURE_INTERNAL_H
# define CAPTURE_INTERNAL_H

# include <stdbool.h>
# include <stddef.h>

typedef struct s_capture_buffer
{
	unsigned char	*data;
	size_t		length;
	size_t		capacity;
	size_t		limit;
	bool			truncated;
}   t_capture_buffer;

void	capture_buffer_init(t_capture_buffer *buffer, size_t limit);
int	capture_buffer_append(t_capture_buffer *buffer,
		const unsigned char *data, size_t length);
int	capture_buffer_read_fd(t_capture_buffer *buffer, int fd, bool *eof);
unsigned char	*capture_buffer_take(t_capture_buffer *buffer);
void	capture_buffer_destroy(t_capture_buffer *buffer);

#endif

