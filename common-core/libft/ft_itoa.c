/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 14:46:36 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/03 12:29:23 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	int_counter(int n)
{
	int	counter;

	counter = 0;
	if (n <= 0)
		counter++;
	while (n != 0)
	{
		n /= 10;
		counter++;
	}
	return (counter);
}

static void	put_number(char *str, long nb, int len)
{
	if (nb < 0)
		nb = -nb;
	while (nb >= 10)
	{
		str[--len] = nb % 10 + '0';
		nb /= 10;
	}
	str[--len] = nb + '0';
}

char	*ft_itoa(int n)
{
	long	nb;
	int		len;
	char	*str;

	nb = n;
	len = int_counter(nb);
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	put_number(str, nb, len);
	if (n < 0)
		str[0] = '-';
	return (str);
}
