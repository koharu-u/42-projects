/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_params.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 14:36:36 by kpiwan            #+#    #+#             */
/*   Updated: 2026/05/14 14:53:36 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	main(int argc, char *argv[])
{
	int		i;
	char	*params;

	i = 1;
	while (i < argc)
	{
		params = argv[i];
		while (*params)
			write(1, params++, 1);
		write(1, "\n", 1);
		i++;
	}
	return (0);
}
