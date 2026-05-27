/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extras.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchantas <tchantasarn@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 15:02:43 by tchantas          #+#    #+#             */
/*   Updated: 2026/05/17 12:47:31 by tchantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rush01.h"

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	print_error(void)
{
	write(1, "Error\n", 6);
}

void	print_grid(int **grid, int n)
{
	int	r;
	int	c;

	r = 0;
	while (r < n)
	{
		c = 0;
		while (c < n)
		{
			ft_putchar('0' + grid[r][c]);
			if (c < n - 1)
				ft_putchar(' ');
			c++;
		}
		ft_putchar('\n');
		r++;
	}
}

/*
** Counts how many buildings are visible walking from start with the given
** step (positive = forward, negative = backward).  A building is visible
** only when it is taller than all previously seen buildings.
** The flat-array layout (data + i*n per row) makes step = ±n valid for cols.
*/
int	count_visible(int *start, int step, int n)
{
	int	visible;
	int	tallest;
	int	i;

	visible = 0;
	tallest = 0;
	i = 0;
	while (i < n)
	{
		if (*(start + i * step) > tallest)
		{
			tallest = *(start + i * step);
			visible++;
		}
		i++;
	}
	return (visible);
}
