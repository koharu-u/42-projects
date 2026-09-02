/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 16:34:03 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/02 16:44:43 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#import "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	const unsigned char	*chr_src;
	unsigned char		*chr_dest;

	i = 0;
	chr_dest = (unsigned char *)dest;
	chr_src = (const unsigned char *)src;
	while (i < n)
	{
		chr_dest[i] = chr_src[i];
		i++;
	}
	return (dest);
}
