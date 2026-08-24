/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_iterative_factorial.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 10:54:24 by kpiwan            #+#    #+#             */
/*   Updated: 2026/05/17 11:16:04 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_iterative_factorial(int nb)
{
	int	i;
	int	nb_rtn;

	if (nb == 0)
		return (1);
	if (nb < 0)
		return (0);
	i = 2;
	nb_rtn = 1;
	while (i <= nb)
	{
		nb_rtn = nb_rtn * i;
		i++;
	}
	return (nb_rtn);
}
