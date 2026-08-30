/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:06:26 by kpiwan            #+#    #+#             */
/*   Updated: 2026/08/30 14:23:12 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *nptr)
{
	int	symbol;
	int	rtn_num;

	rtn_num = 0;
	symbol = 1;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			symbol = -symbol;
		nptr++;
	}
	while (*nptr && (*nptr >= '0' && *nptr <= '9'))
		rtn_num = (rtn_num * 10) + (*nptr++ - '0');
	return (rtn_num * symbol);
}
