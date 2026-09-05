/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 16:26:21 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/05 16:49:32 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i_dst;
	size_t	i_src;

	i_dst = 0;
	while (i_dst < size && dst[i_dst])
		i_dst++;
	src_len = ft_strlen(src);
	dst_len = i_dst;
	if (size == dst_len)
		return (size + src_len);
	i_src = 0;
	while (src[i_src] && (i_dst < size - 1))
	{
		dst[i_dst] = src[i_src];
		i_src++;
		i_dst++;
	}
	dst[i_dst] = '\0';
	return (dst_len + src_len);
}
