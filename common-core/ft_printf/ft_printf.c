/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 08:39:46 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/19 11:02:21 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_handle_text(int type, va_list *ap)
{
	char	*s;

	if (type == 7)
	{
		if (ft_putchar_fd('%', 1) == -1)
			return (-1);
		return (1);
	}
	if (type == 1)
	{
		if (ft_putchar_fd((char)va_arg(*ap, int), 1) == -1)
			return (-1);
		return (1);
	}
	s = va_arg(*ap, char *);
	if (!s)
	{
		if (ft_putstr_fd("(null)", 1) == -1)
			return (-1);
		return (6);
	}
	if (ft_putstr_fd(s, 1) == -1)
		return (-1);
	return (ft_strlen(s));
}

static int	ft_handle_number(int type, va_list *ap)
{
	if (type == 0)
		return (ft_numpnt(va_arg(*ap, int)));
	if (type == 3)
		return (ft_pntpnt(va_arg(*ap, void *)));
	if (type == 4)
		return (ft_unumpnt(va_arg(*ap, unsigned int)));
	if (type == 5)
		return (ft_putnbr_base(va_arg(*ap, unsigned int), "0123456789abcdef"));
	if (type == 6)
		return (ft_putnbr_base(va_arg(*ap, unsigned int), "0123456789ABCDEF"));
	return (0);
}

static int	ft_handle_format(const char *format, size_t i, va_list *ap)
{
	int	type;

	type = ft_checker(format, i);
	if (type == 1 || type == 2 || type == 7)
		return (ft_handle_text(type, ap));
	return (ft_handle_number(type, ap));
}

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	size_t	i;
	int		len;
	int		wri;

	i = 0;
	len = 0;
	va_start(ap, format);
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			wri = ft_handle_format(format, i, &ap);
			i++;
		}
		else
			wri = ft_putchar_fd(format[i], 1);
		if (wri == -1)
			return (va_end(ap), -1);
		len += wri;
		i++;
	}
	va_end(ap);
	return (len);
}
