/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 14:04:58 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/19 17:00:47 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putstr_fd(char *s, int fd)
{
	size_t	len;
	size_t	total;
	int		wri;

	len = ft_strlen(s);
	total = 0;
	if (len == 0)
		return (write(fd, s, 0));
	while (total < len)
	{
		wri = write(fd, s + total, len - total);
		if (wri <= 0)
			return (-1);
		total += wri;
	}
	return ((int)total);
}
