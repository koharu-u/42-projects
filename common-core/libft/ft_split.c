/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 08:40:36 by kpiwan            #+#    #+#             */
/*   Updated: 2026/08/30 15:49:03 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	is_charset(char chr, char *charset)
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

int	word_counter(char *str, char *charset)
{
	int	counter;
	int	i;

	counter = 0;
	i = 0;
	while (str[i])
	{
		if (is_charset(str[i], charset) == 0 && (is_charset(str[i + 1],
					charset) == 1 || str[i + 1] == '\0'))
			counter++;
		i++;
	}
	return (counter);
}

char	**ft_split(char const *s, char *c)
{
	char	**rtnptr;
	int		i;
	int		j;
	int		wcount;

	i = 0;
	j = 0;
	rtnptr = malloc(sizeof(char *) * (word_counter(str, charset) + 1));
	if (rtnptr == NULL)
		return (NULL);
	while (str[i])
	{
		while (str[i] && is_charset(str[i], charset) == 1)
			i++;
		if (str[i] == '\0')
			break ;
		wcount = 0;
		while (str[i + wcount] && is_charset(str[i + wcount], charset) == 0)
			wcount++;
		rtnptr[j] = ft_strdup(&str[i]);
		j++;
		i += wcount;
	}
	rtnptr[j] = 0;
	return (rtnptr);
}
