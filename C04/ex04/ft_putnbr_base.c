/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 22:20:32 by kpiwan            #+#    #+#             */
/*   Updated: 2026/05/13 23:53:25 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putnbr_base(int nbr, char *base);

void	print_nb(long nmbr, char *bs, int len)
{
	if (nmbr >= len)
	{
		print_nb(nmbr / len, bs, len);
	}
	write(1, &bs[nmbr % len], 1);
}

void	ft_putnbr_base(int nbr, char *base)
{
	unsigned int	cnt_base;
	long			mbr_long;

	cnt_base = 0;
	mbr_long = nbr;
	if (nbr < 0)
	{
		mbr_long = -mbr_long;
		write(1, "-", 1);
	}
	while (base[cnt_base])
	{
		if ((base[cnt_base] == base[cnt_base + 1]) || ((base[cnt_base] == '-')
				|| (base[cnt_base] == '+')))
			return ;
		cnt_base++;
	}
	if (cnt_base < 2)
		return ;
	print_nb(mbr_long, base, cnt_base);
}
