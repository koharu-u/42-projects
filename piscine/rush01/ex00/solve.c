/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchantas <tchantasarn@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 15:07:46 by tchantas          #+#    #+#             */
/*   Updated: 2026/05/17 14:44:35 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** clues layout (flat array, length 4*n):
**   clues[0..n-1]     = top    (col c viewed from top)
**   clues[n..2n-1]    = bot    (col c viewed from bottom)
**   clues[2n..3n-1]   = left   (row r viewed from left)
**   clues[3n..4n-1]   = right  (row r viewed from right)
*/

#include "rush01.h"

/*
** Partial clue check after placing grid[r][c].
** Left / top: partial check is valid because filled cells are already between
**   the clue edge and the empty cells - vis can only grow, so vis is a real
**   lower bound.
** Right / bottom: only checked when the row / column is complete because the
**   empty cells lie between the edge and the filled portion and could block
**   filled cells, making vis an incorrect lower bound for a partial row/col.
*/
static int	check_clues(int **grid, int pos, int n, int *clues)
{
	int	r;
	int	c;
	int	vis;

	r = pos / n;
	c = pos % n;
	vis = count_visible(&grid[r][0], 1, c + 1);
	if (clues[2 * n + r] < vis || clues[2 * n + r] > vis + n - c - 1)
		return (0);
	if (c == n - 1 && clues[3 * n + r] != count_visible(&grid[r][c], -1, n))
		return (0);
	vis = count_visible(&grid[0][c], n, r + 1);
	if (clues[c] < vis || clues[c] > vis + n - r - 1)
		return (0);
	if (r == n - 1 && clues[n + c] != count_visible(&grid[r][c], -n, n))
		return (0);
	return (1);
}

int	is_safe(int **grid, int pos, int n, int *clues)
{
	int	r;
	int	c;
	int	v;
	int	i;

	r = pos / n;
	c = pos % n;
	v = grid[r][c];
	i = 0;
	while (i < c)
		if (grid[r][i++] == v)
			return (0);
	i = 0;
	while (i < r)
		if (grid[i++][c] == v)
			return (0);
	return (check_clues(grid, pos, n, clues));
}

int	solve(int **grid, int pos, int n, int *clues)
{
	int	r;
	int	c;
	int	v;

	if (pos == n * n)
		return (1);
	r = pos / n;
	c = pos % n;
	v = 1;
	while (v <= n)
	{
		grid[r][c] = v;
		if (is_safe(grid, pos, n, clues) && solve(grid, pos + 1, n, clues))
			return (1);
		grid[r][c] = 0;
		v++;
	}
	return (0);
}
