/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 10:41:41 by kpiwan            #+#    #+#             */
/*   Updated: 2026/05/12 10:46:50 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

unsigned int	ft_strlcat(char *dest, char *src, unsigned int size);

unsigned int	stringlen(char *chk_char)
{
	unsigned int	len_str;

	len_str = 0;
	while (chk_char[len_str])
		len_str++;
	return (len_str);
}

unsigned int	ft_strlcat(char *dest, char *src, unsigned int size)
{
	unsigned int	cnt_dest;
	unsigned int	cnt_src;
	unsigned int	i;
	unsigned int	j;

	i = 0;
	j = 0;
	cnt_dest = stringlen(dest);
	cnt_src = stringlen(src);
	if (size <= cnt_dest)
		return (size + cnt_src);
	i = cnt_dest;
	while ((src[j]) && (i < size - 1))
	{
		dest[i] = src[j];
		j++;
		i++;
	}
	dest[i] = '\0';
	return (cnt_dest + cnt_src);
}
