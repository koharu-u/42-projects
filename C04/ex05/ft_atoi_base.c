/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 22:20:32 by kpiwan            #+#    #+#             */
/*   Updated: 2026/05/14 10:54:29 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	ft_putnbr_base(int nbr, char *base);

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

int	chk_base(char str, char *base)
{
	int	i;

	i = 0;
	while (base[i])
	{
		if (str == base[i])
			return (i);
		else
			i++;
	}
	return (-1);
}

int	ft_atoi_base(char *str, char *base)
{
	int				int_rtn;
	int				symbol;
	unsigned int	cnt_base;

	int_rtn = 0;
	symbol = 1;
	cnt_base = 0;
	while (base[cnt_base])
		cnt_base++;
	if (detect_invalid(base, cnt_base) == 1)
		return (0);
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	while ((*str == '-') || (*str == '+'))
	{
		if (*str == '-')
			symbol = -symbol;
		str++;
	}
	while (chk_base(*str, base) != -1)
	{
		int_rtn = (int_rtn * cnt_base) + ((chk_base(*str, base)));
		str++;
	}
	return (int_rtn * symbol);
}
