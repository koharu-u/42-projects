/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 21:38:08 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/03 21:57:53 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t			s_len;
	unsigned int	i_s;
	char			*ptr;

	s_len = ft_strlen(s);
	ptr = malloc(s_len + 1);
	if (!ptr)
		return (NULL);
	i_s = 0;
	while (s[i_s])
	{
		ptr[i_s] = (*f)(i_s, s[i_s]);
		i_s++;
	}
	ptr[i_s] = '\0';
	return (ptr);
}
