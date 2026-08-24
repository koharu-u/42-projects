/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 22:20:32 by kpiwan            #+#    #+#             */
/*   Updated: 2026/05/14 09:16:19 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putnbr_base(int nbr, char *base);

void	print_nb(long nmbr, char *bs, int len)
{
	if (nmbr >= len)
		print_nb(nmbr / len, bs, len);
	write(1, &bs[nmbr % len], 1);
}

int	detect_invalid(char *base, int cnt_base)
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

void	ft_putnbr_base(int nbr, char *base)
{
	unsigned int	cnt_base;
	long			mbr_long;
	int				is_invalid;

	cnt_base = 0;
	mbr_long = nbr;
	while (base[cnt_base])
		cnt_base++;
	is_invalid = detect_invalid(base, cnt_base);
	if (is_invalid == 1)
		return ;
	if (nbr < 0)
	{
		mbr_long = -mbr_long;
		write(1, "-", 1);
	}
	print_nb(mbr_long, base, cnt_base);
}
