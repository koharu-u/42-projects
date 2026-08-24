/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchantas <tchantasarn@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 22:04:27 by tchantas          #+#    #+#             */
/*   Updated: 2026/05/17 12:47:29 by tchantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rush01.h"

/*
** Parses argv[1] into a flat clues array and sets *n.
** Expected format: digits 1-9 separated by single spaces, count divisible by 4,
** n = count/4 in range [1,9].
*/
int	parse_input(int argc, char **argv, int *n, int *clues)
{
	char	*s;
	int		count;
	int		i;

	if (argc != 2)
		return (0);
	s = argv[1];
	count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] < '1' || s[i] > '9' || count >= 36)
			return (0);
		clues[count++] = s[i++] - '0';
		if (!s[i])
			break ;
		if (s[i++] != ' ')
			return (0);
	}
	*n = count / 4;
	if (*n < 1 || *n > 9 || count % 4 != 0)
		return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	int		clues[36];
	int		n;
	int		*data;
	int		**grid;
	int		i;

	if (!parse_input(argc, argv, &n, clues))
		return (print_error(), 0);
	data = malloc(n * n * sizeof(int));
	grid = malloc(n * sizeof(int *));
	if (!data || !grid)
		return (print_error(), 0);
	i = 0;
	while (i < n)
	{
		grid[i] = data + i * n;
		i++;
	}
	if (solve(grid, 0, n, clues))
		print_grid(grid, n);
	else
		print_error();
	free(data);
	free(grid);
	return (0);
}
