/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 13:08:17 by kpiwan            #+#    #+#             */
/*   Updated: 2026/08/30 14:56:25 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	counter;

	counter = 0;
	if (n == 0)
		return (0);
	while ((counter < (n - 1)) && (s1[counter] == s2[counter] && (s1[counter])))
		counter++;
	return ((unsigned char)s1[counter] - (unsigned char)s2[counter]);
}
