/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_range.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 12:12:16 by kpiwan            #+#    #+#             */
/*   Updated: 2026/05/19 13:36:23 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	*ft_range(int min, int max)
{
	int	*ptr;
	int	cnt;
	int	size;

	if (min >= max)
		return (0);
	size = max - min;
	ptr = malloc(sizeof(int) * size);
	if (ptr == NULL)
	{
		return (NULL);
	}
	cnt = 0;
	while (cnt < size)
	{
		ptr[cnt] = min + cnt;
		cnt++;
	}
	return (ptr);
}
