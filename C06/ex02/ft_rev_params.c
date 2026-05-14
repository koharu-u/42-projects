/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rev_params.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 14:36:36 by kpiwan            #+#    #+#             */
/*   Updated: 2026/05/14 15:25:45 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	rev_params(int argc, char *argv[], int i)
{
	char	*params;

	if (i >= argc)
		return ;
	rev_params(argc, argv, i + 1);
	params = argv[i];
	while (*params)
		write(1, params++, 1);
	write(1, "\n", 1);
	return ;
}

int	main(int argc, char *argv[])
{
	if (argc > 1)
		rev_params(argc, argv, 1);
	return (0);
}
