/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 15:38:33 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/04 14:03:01 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strncpy(char *dest, const char *str, size_t number)
{
	size_t	i;

	i = 0;
	while (i < number)
	{
		dest[i] = str[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strdup(const char *s)
{
	char	*ptr;
	size_t	number;

	number = ft_strlen(s);
	ptr = malloc(sizeof(char) * (number + 1));
	if (ptr == NULL)
		return (NULL);
	ft_strncpy(ptr, s, number);
	return (ptr);
}
