/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_iterative_power.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 11:30:55 by kpiwan            #+#    #+#             */
/*   Updated: 2026/05/17 11:38:32 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_iterative_power(int nb, int power)
{
	int	i;
	int	opt;

	if (power < 0)
		return (0);
	if (power == 0)
		return (1);
	i = 2;
	opt = nb;
	while (i <= power)
	{
		opt = opt * nb;
		i++;
	}
	return (opt);
}
