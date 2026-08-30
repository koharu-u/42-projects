/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 13:08:19 by kpiwan            #+#    #+#             */
/*   Updated: 2026/08/30 14:37:58 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*ptr;
	size_t				i_n;

	i_n = 0;
	ptr = (const unsigned char *)s;
	while (n > i_n)
	{
		if (*ptr == (unsigned char)c)
		{
			return ((void *)s);
		}
		s++;
		ptr++;
		i_n++;
	}
	return (NULL);
}
