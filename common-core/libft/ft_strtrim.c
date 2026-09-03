/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 22:13:25 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/03 22:31:19 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_charset(char chr, const char *charset)
{
	int	i;

	i = 0;
	while (charset[i])
	{
		if (charset[i] == chr)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i_srt;
	size_t	i_end;
	size_t	i_ptr;
	char	*rst_ptr;

	i_srt = 0;
	i_end = ft_strlen(s1);
	while (s1[i_srt] && is_charset(s1[i_srt], set))
		i_srt++;
	while (i_end > i_srt && is_charset(s1[i_end - 1], set))
		i_end--;
	rst_ptr = malloc(i_end - i_srt + 1);
	if (!rst_ptr)
		return (NULL);
	i_ptr = 0;
	while (i_srt < i_end)
	{
		rst_ptr[i_ptr] = s1[i_srt];
		i_srt++;
		i_ptr++;
	}
	rst_ptr[i_ptr] = '\0';
	return (rst_ptr);
}
