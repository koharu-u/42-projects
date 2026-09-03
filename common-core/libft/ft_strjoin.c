/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 16:58:40 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/02 17:17:18 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	size_t	n_s1;
	size_t	n_s2;
	char	*ptr;

	n_s1 = ft_strlen(s1);
	n_s2 = ft_strlen(s2);
	ptr = malloc(n_s1 + n_s2 + 1);
	if (!ptr)
		return (NULL);
	i = 0;
	j = 0;
	while (n_s1 > j)
		ptr[i++] = s1[j++];
	j = 0;
	while (n_s2 > j)
		ptr[i++] = s2[j++];
	ptr[i] = '\0';
	return (ptr);
}
