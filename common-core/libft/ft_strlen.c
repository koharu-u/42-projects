/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 22:35:50 by kpiwan            #+#    #+#             */
/*   Updated: 2026/08/29 17:22:32 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

size_t	ft_strlen(const char *s);

size_t	ft_strlen(const char *s)
{
	size_t	cnt;

	cnt = 0;
	while (*s++)
		cnt++;
	return (cnt);
}
