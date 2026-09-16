/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 08:39:46 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/16 14:08:39 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	size_t	i_pos;
	size_t	len_print;
	char	c;
	char	*s;

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
				s = va_arg(ap, char *);
				len_print = ft_strlen(s);
				ft_putstr_fd(s, 1);
				i_pos++;
			}
			else if (ft_checker(format, i_pos) == 4)
			{
				len_print += ft_unumpnt(va_arg(ap, unsigned int));
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
	int	counter;

	counter = ft_printf("SIX SEVEN is %i%% or %d?\n", 67, 69);
	ft_printf("[TEST] Expected: 24, got %d\n", counter);
	counter = ft_printf("EDGE CASE: % \n");
	ft_printf("[TEST] Expected: 24, got %d\n", counter);
	counter = ft_printf("%u\n", -1);
	ft_printf("[TEST] Expected: 11, got %d\n", counter);
	counter = ft_printf("%s\n", "HEHEHE");
	ft_printf("[TEST] Expected: 7, got %d\n", counter);
	return (0);
}
