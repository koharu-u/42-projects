/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 08:39:46 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/15 22:44:29 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	size_t	i_pos;
	size_t	len_print;
	char	c;

	i_pos = 0;
	len_print = 0;
	va_start(ap, format);
	while (format[i_pos])
	{
		if (format[i_pos] == '%')
		{
			if (ft_checker(format, i_pos) == 7)
			{
				ft_putchar_fd('%', 1);
				len_print++;
				i_pos++;
			}
			else if (ft_checker(format, i_pos) == 0)
			{
				len_print += ft_numpnt(va_arg(ap, int));
				i_pos++;
			}
			else if (ft_checker(format, i_pos) == 1)
			{
				c = (char)va_arg(ap, int);
				ft_putchar_fd(c, 1);
				len_print++;
				i_pos++;
			}
			else if (ft_checker(format, i_pos) == 2)
			{
				ft_putstr_fd(va_arg(ap, char *), 1);
				len_print++;
				i_pos++;
			}
		}
		else
		{
			ft_putchar_fd(format[i_pos], 1);
			len_print++;
		}
		i_pos++;
	}
	va_end(ap);
	return ((int)len_print);
}

#include <stdio.h>

int	main(void)
{
	ft_printf("SIX SEVEN is %d%% or %d?\n", 67, 69);
	ft_printf("%s\n", "HEHEHE");
	printf("SIX SEVEN is %d%% or %d?\n", 67, 69);
	return (0);
}
