/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 13:58:14 by kpiwan            #+#    #+#             */
/*   Updated: 2026/05/13 22:16:27 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	ft_atoi(char *str);

int	ft_atoi(char *str)
{
	int	int_rtn;
	int	symbol;

	int_rtn = 0;
	symbol = 1;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	while ((*str == '-') || (*str == '+'))
	{
		if (*str == '-')
			symbol = -symbol;
		str++;
	}
	while (*str && ((*str >= '0') && (*str <= '9')))
		int_rtn = (int_rtn * 10) + ((*str++ - '0'));
	return (int_rtn * symbol);
}
