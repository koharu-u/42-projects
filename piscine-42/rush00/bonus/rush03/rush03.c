/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rush03.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 11:48:04 by kpiwan            #+#    #+#             */
/*   Updated: 2026/05/09 20:56:21 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	rush(int x, int y);
void	ft_putchar(char c);

void	rush(int x, int y)
{
	int	i;
	int	j;

	j = 1;
	while (j <= y)
	{
		i = 1;
		while (i <= x)
		{
			if (((i == 1 && j == 1) || (i == 1 && j == y)))
				ft_putchar('A');
			else if ((i == x && j == 1) || (i == x && j == y))
				ft_putchar('C');
			else if (((j > 1 && j < y) && (i == 1 || i == x)) || ((i > 1
						&& i < x) && (j == 1 || j == y)))
				ft_putchar('B');
			else if ((i > 1 && i < x) && (j != 1 && j != y))
				ft_putchar(' ');
			if (i == x && j != y)
				ft_putchar('\n');
			i++;
		}
		j++;
	}
}
