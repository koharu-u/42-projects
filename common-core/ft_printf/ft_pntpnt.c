/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pntpnt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 20:48:52 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/17 21:54:49 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_pntpnt(void *ptr)
{
	int			counter;
	char		*hex;
	uintptr_t	address;
	char		*nl;

	if (!ptr)
	{
		nl = "(nil)";
		ft_putstr_fd(nl, 1);
		return (5);
	}
	address = (uintptr_t)ptr;
	counter = 0;
	hex = "0123456789abcdef";
	ft_putchar_fd('0', 1);
	ft_putchar_fd('x', 1);
	counter += 2;
	counter += ft_putnbr_base(address, hex);
	return (counter);
}
