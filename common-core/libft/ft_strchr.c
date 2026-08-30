/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:47:57 by kpiwan            #+#    #+#             */
/*   Updated: 2026/08/30 13:59:32 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(const char *s, int c)
{
	while (*s && (((unsigned char)*s) != (unsigned char)c))
		s++;
	if ((unsigned char)*s == (unsigned char)c)
		return ((char *)s);
	return (0);
}
