/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rush01.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchantas <tchantasarn@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 12:47:47 by tchantas          #+#    #+#             */
/*   Updated: 2026/05/17 14:34:50 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUSH01_H
# define RUSH01_H

# include <stdlib.h>
# include <unistd.h>

void	ft_putchar(char c);
void	print_error(void);
void	print_grid(int **grid, int n);
int		count_visible(int *start, int step, int n);
int		parse_input(int argc, char **argv, int *n, int *clues);
int		is_safe(int **grid, int pos, int n, int *clues);
int		solve(int **grid, int pos, int n, int *clues);

#endif
