#ifndef LIBFT_API_H
# define LIBFT_API_H

# include <stddef.h>

size_t	ft_strlen(const char *string);
int		ft_isalpha(int character);
int		ft_isdigit(int character);
void	*ft_memset(void *destination, int value, size_t length);
void	*ft_memcpy(void *destination, const void *source, size_t length);

#endif
