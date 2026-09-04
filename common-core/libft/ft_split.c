/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 08:40:36 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/04 14:16:26 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	word_counter(char const *str, char c)
{
	int	counter;
	int	i;

	counter = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] != c && (str[i + 1] == c || str[i + 1] == '\0'))
			counter++;
		i++;
	}
	return (counter);
}

static void	free_split(char **split, int count)
{
	while (count > 0)
	{
		count--;
		free(split[count]);
	}
	free(split);
}

static int	fill_split(char **rtnptr, const char *s, char c)
{
	int	i;
	int	j;
	int	wcount;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i] == '\0')
			break ;
		wcount = 0;
		while (s[i + wcount] && s[i + wcount] != c)
			wcount++;
		rtnptr[j++] = ft_substr(s, i, wcount);
		if (rtnptr[j - 1] == NULL)
		{
			free_split(rtnptr, j - 1);
			return (0);
		}
		i += wcount;
	}
	rtnptr[j] = NULL;
	return (1);
}

char	**ft_split(const char *s, char c)
{
	char	**rtnptr;

	if (s == NULL)
		return (NULL);
	rtnptr = malloc(sizeof(char *) * (word_counter(s, c) + 1));
	if (rtnptr == NULL)
		return (NULL);
	if (fill_split(rtnptr, s, c) == 0)
		return (NULL);
	return (rtnptr);
}
