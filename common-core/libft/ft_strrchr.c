/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 09:31:15 by kpiwan            #+#    #+#             */
/*   Updated: 2026/08/30 12:19:42 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(const char *s, int c);

char	*ft_strrchr(const char *s, int c)
{
	int	pos;
	int	dt_pos;

	pos = 0;
	dt_pos = -1;
	while (s[pos])
	{
		if (s[pos] == c)
			dt_pos = pos;
		pos++;
	}
	if (s[pos] == c)
		return (s + dt_pos);
	if (dt_pos != -1)
		return (s + dt_pos);
	return (0);
}
