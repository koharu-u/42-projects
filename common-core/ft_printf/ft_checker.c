/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_checker.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 08:39:46 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/17 22:20:25 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_checker(const char *format, size_t position)
{
	if (format[position + 1] == 'd' || format[position + 1] == 'i')
		return (0);
	if (format[position + 1] == 'c')
		return (1);
	if (format[position + 1] == 's')
		return (2);
	if (format[position + 1] == 'p')
		return (3);
	if (format[position + 1] == 'u')
		return (4);
	if (format[position + 1] == 'x')
		return (5);
	if (format[position + 1] == 'X')
		return (6);
	if (format[position + 1] == '%')
		return (7);
	return (-1);
}
