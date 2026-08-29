/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:06:26 by kpiwan            #+#    #+#             */
/*   Updated: 2026/08/29 12:29:44 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *nptr);

int	ft_atoi(const char *nptr)
{
	int	symbol;
	int	rtn_num;

	rtn_num = 0;
	symbol = 1;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		*str++;
	if (*str == 45)
		rtn_num = -rtn_num;
	while (*nptr && (*nptr >= 9 && *nptr <= 13))
		rtn_num = (rtn_num * 10) + (*nptr++ - '0');
	return (rtn_num * symbol);
}
