/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_is_uppercase.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 23:08:00 by kpiwan            #+#    #+#             */
/*   Updated: 2026/05/09 23:28:36 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	ft_str_is_uppercase(char *str);

int	ft_str_is_uppercase(char *str)
{
	int	i;
	int	is_uppercase;

	i = 0;
	is_uppercase = 1;
	while (str[i])
	{
		if (!((str[i] >= 65 && str[i] <= 90)))
		{
			is_uppercase = 0;
		}
		i++;
	}
	return (is_uppercase);
}
