/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_base2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 15:02:06 by kpiwan            #+#    #+#             */
/*   Updated: 2026/05/25 15:03:49 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		mbr_long = -mbr_long;
}
