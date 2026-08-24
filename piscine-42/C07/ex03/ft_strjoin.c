/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 14:00:54 by kpiwan            #+#    #+#             */
/*   Updated: 2026/05/19 16:21:40 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	joinit(int size, char **str, char *sep, char *rtn)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	while (i < size)
	{
		j = 0;
		while (str[i][j])
			rtn[k++] = str[i][j++];
		j = 0;
		if (i < size - 1)
		{
			while (sep[j])
				rtn[k++] = sep[j++];
		}
		i++;
	}
	rtn[k] = '\0';
	return ;
}

char	*ft_strjoin(int size, char **strs, char *sep)
{
	char	*rtn;
	int		i;
	int		total_len;

	total_len = 0;
	if (size <= 0)
	{
		rtn = malloc(sizeof(char) * 1);
		rtn[0] = '\0';
		return (rtn);
	}
	i = 0;
	while (i < size)
	{
		total_len = total_len + ft_strlen(strs[i]);
		i++;
	}
	rtn = malloc(sizeof(char) * (total_len + 1 + ((size - 1)
					* ft_strlen(sep))));
	if (rtn == NULL)
		return (NULL);
	joinit(size, strs, sep, rtn);
	return (rtn);
}
