/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 16:49:51 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/02 16:55:18 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				i;
	const unsigned char	*chr_s1;
	const unsigned char	*chr_s2;

	i = 0;
	chr_s1 = (const unsigned char *)s1;
	chr_s2 = (const unsigned char *)s2;
	while (i < n)
	{
		if (chr_s2[i] != chr_s1[i])
			return (chr_s1[i] - chr_s2[i]);
		i++;
	}
	return (0);
}
