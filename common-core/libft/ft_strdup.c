/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 15:38:33 by kpiwan            #+#    #+#             */
/*   Updated: 2026/08/30 15:44:39 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strncpy(char *dest, char *str, int number)
{
	int	i;

	i = 0;
	while (str[i] && i < number)
	{
		dest[i] = str[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strdup(char *src)
{
	char	*ptr;
	size_t	number;

	number = ft_strlen(src);
	ptr = malloc(sizeof(char) * (number + 1));
	if (ptr == NULL)
		return (NULL);
	ptr = ft_strncpy(ptr, src, number);
	return (ptr);
}
