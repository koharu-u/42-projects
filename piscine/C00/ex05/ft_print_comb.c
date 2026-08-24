/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_comb.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:19:49 by kpiwan            #+#    #+#             */
/*   Updated: 2026/05/07 09:59:51 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putchar(char c);
void	ft_print_comb(void);

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_print_comb(void)
{
	int	i[4];

	i[0] = 0;
	while (i[0] <= 7)
	{
		i[1] = i[0] + 1;
		while (i[1] <= 8)
		{
			i[2] = i[1] + 1;
			while (i[2] <= 9)
			{
				if (!(i[0] == 0 && i[1] == 1 && i[2] == 2))
					write(1, ", ", 2);
				i[3] = 48 + i[0];
				ft_putchar(i[3]);
				i[3] = 48 + i[1];
				ft_putchar(i[3]);
				i[3] = 48 + i[2];
				ft_putchar(i[3]);
				i[2]++;
			}
			i[1]++;
		}
		i[0]++;
	}
}

// int	main(void)
// {
// 	ft_print_comb();
// 	return (0);
// }
