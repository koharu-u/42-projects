/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 09:31:15 by kpiwan            #+#    #+#             */
/*   Updated: 2026/08/26 09:50:47 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

const char	*ft_strrchr(const char *str, int chr)
{
	int	pos;
	int	dt_pos;

	pos = 0;
	dt_pos = -1;
	while (str[pos])
	{
		if (str[pos] == chr)
			dt_pos = pos;
		pos++;
	}
	if (str[pos] == chr)
		return (str + dt_pos);
	if (dt_pos != -1)
		return (str + dt_pos);
	return (0);
}
