/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 22:20:32 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/19 11:44:52 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	print_nb(uintptr_t nmbr, char *bs, int len)
{
	int	count;
	int	ret;

	count = 0;
	if (nmbr >= (uintptr_t)len)
	{
		ret = print_nb(nmbr / len, bs, len);
		if (ret == -1)
			return (-1);
		count += ret;
	}
	if (write(1, &bs[nmbr % len], 1) == -1)
		return (-1);
	return (count + 1);
}

static int	detect_invalid(char *base, int cnt_base)
{
	int	cur_pos;
	int	loop_pos;

	cur_pos = 0;
	loop_pos = 0;
	if (cnt_base < 2)
		return (1);
	while (cur_pos < cnt_base)
	{
		loop_pos = 0;
		while (base[loop_pos])
		{
			if (((base[loop_pos] == base[cur_pos])) && (loop_pos != cur_pos))
				return (1);
			else if (((base[loop_pos] == '-') || (base[loop_pos] == '+'))
				|| (((base[loop_pos] >= 9) && (base[loop_pos] <= 13))
					|| (base[loop_pos] == 32)))
				return (1);
			else
				loop_pos++;
		}
		cur_pos++;
	}
	return (0);
}

int	ft_putnbr_base(uintptr_t nbr, char *base)
{
	unsigned int	cnt_base;
	int				is_invalid;

	cnt_base = 0;
	while (base[cnt_base])
		cnt_base++;
	is_invalid = detect_invalid(base, cnt_base);
	if (is_invalid == 1)
		return (0);
	return (print_nb(nbr, base, cnt_base));
}
