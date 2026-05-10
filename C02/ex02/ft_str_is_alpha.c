/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_is_alpha.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 23:08:00 by kpiwan            #+#    #+#             */
/*   Updated: 2026/05/10 14:10:12 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	ft_str_is_alpha(char *str);

int	ft_str_is_alpha(char *str)
{
	int	i;
	int	is_alpha;

	i = 0;
	is_alpha = 1;
	while (str[i])
	{
		if (!((str[i] >= 65 && str[i] <= 90) || (str[i] >= 97
					&& str[i] <= 122)))
		{
			is_alpha = 0;
		}
		i++;
	}
	return (is_alpha);
}
