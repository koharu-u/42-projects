/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 09:31:15 by kpiwan            #+#    #+#             */
/*   Updated: 2026/08/30 14:10:53 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	index;

	index = 0;
	while (s[index] != '\0')
		index++;
	while (index > 0)
	{
		if (((unsigned char)s[index]) == (unsigned char)c)
			return ((char *)&s[index]);
		index--;
	}
	if ((unsigned char)s[0] == (unsigned char)c)
		return ((char *)&s[0]);
	return (0);
}
