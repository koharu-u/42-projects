/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 13:58:14 by kpiwan            #+#    #+#             */
/*   Updated: 2026/05/13 15:54:40 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	ft_atoi(char *str);

int	int_add(int innt)
{
	int_rtn = (int_rtn) + ((str[counter] - '0') * digit);
	digit = digit * 10;
	counter++;
}

int	ft_atoi(char *str)
{
	int	counter;
	int	int_rtn;
	int	digit;
	int	symbol;

	counter = 0;
	int_rtn = 0;
	digit = 1;
	while ((str[counter] >= 9 && str[counter] <= 13) || str[counter] == 32)
		counter++;
	symbol = 1;
	while ((str[counter] == '-') || (str[counter] == '+'))
	{
		if (str[counter] == '-')
			symbol = -symbol;
		counter++;
	}
	while (str[counter] && ((str[counter] >= '0') && (str[counter] <= '9')))
	{
	}
	if (symbol == -1)
	{
		int_rtn = -int_rtn;
	}
	return (int_rtn);
}
