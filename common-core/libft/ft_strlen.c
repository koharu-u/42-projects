/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 22:35:50 by kpiwan            #+#    #+#             */
/*   Updated: 2026/08/29 12:49:26 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

size_t	ft_strlen(const char *s);

size_t	ft_strlen(const char *s)
{
	int	counter;

	counter = 0;
	while (*str++)
		counter++;
	return (counter);
}
