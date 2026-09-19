/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unumpnt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 14:46:36 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/19 10:49:09 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	int_counter(unsigned long n)
{
	int	counter;

	counter = 0;
	if (n == 0)
		counter++;
	while (n != 0)
	{
		n /= 10;
		counter++;
	}
	return (counter);
}

static void	put_number(char *str, unsigned long nb, int len)
{
	while (nb >= 10)
	{
		str[--len] = nb % 10 + '0';
		nb /= 10;
	}
	str[--len] = nb + '0';
}

static char	*ft_itoa(unsigned long n, int len)
{
	char	*str;

	str = malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	put_number(str, n, len);
	return (str);
}

int	ft_unumpnt(unsigned int n)
{
	unsigned long	nb;
	int				len;
	char			*str;
	int				i;

	nb = n;
	len = int_counter(nb);
	str = ft_itoa(nb, len);
	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (ft_putchar_fd(str[i], 1) == -1)
		{
			free(str);
			return (-1);
		}
		i++;
	}
	free(str);
	return (len);
}
