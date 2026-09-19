/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pntpnt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 20:48:52 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/19 11:41:37 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_pntpnt(void *ptr)
{
	int			counter;
	char		*hex;
	uintptr_t	address;
	int			wri;

	if (!ptr)
	{
		if (ft_putstr_fd("(nil)", 1) == -1)
			return (-1);
		return (5);
	}
	address = (uintptr_t)ptr;
	counter = 0;
	hex = "0123456789abcdef";
	if (ft_putchar_fd('0', 1) == -1)
		return (-1);
	if (ft_putchar_fd('x', 1) == -1)
		return (-1);
	counter += 2;
	wri = ft_putnbr_base(address, hex);
	if (wri == -1)
		return (-1);
	counter += wri;
	return (counter);
}
