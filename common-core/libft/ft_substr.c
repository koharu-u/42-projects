/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 11:30:18 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/01 13:23:38 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#import "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*ptr;
	unsigned int	i;

	if (ft_strlen(s) <= start)
	{
		ptr = malloc(1);
		if (ptr == NULL)
			return (NULL);
		ptr[0] = '\0';
		return (ptr);
	}
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	ptr = malloc(sizeof(char) * ((len + 1)));
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (s[i + start] && i < len)
	{
		ptr[i] = s[start + i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}
