/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 16:32:32 by kpiwan            #+#    #+#             */
/*   Updated: 2026/05/26 13:57:16 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int		ft_atoi_base(char *str, char *base);
void	ft_putnbr_base(int nbr, char *base);
int		detect_invalid(char *base, int cnt_base);

long	int_size(long nbr, char *base_to)
{
	int	base_len;
	int	size;

	base_len = 0;
	size = 0;
	while (base_to[base_len])
		base_len++;
	if (nbr == 0)
		return (1);
	if (nbr < 0)
	{
		nbr = -nbr;
		size++;
	}
	while (nbr > 0)
	{
		nbr = nbr / base_len;
		size++;
	}
	return (size);
}

int	chk_neg(long nbr)
{
	if (nbr < 0)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

char	*itoa(long nbr, int size, char *base_to)
{
	char	*str;
	int		base_len;
	int		is_neg;

	base_len = 0;
	while (base_to[base_len])
		base_len++;
	is_neg = chk_neg(nbr);
	if (is_neg == 1)
		nbr = -nbr;
	str = malloc(sizeof(char) * (size + 1));
	if (!str)
		return (NULL);
	if (is_neg == 1)
		str[0] = '-';
	str[size--] = '\0';
	while (size >= is_neg)
	{
		str[size] = base_to[nbr % base_len];
		nbr = nbr / base_len;
		size--;
	}
	return (str);
}

char	*ft_convert_base(char *nbr, char *base_from, char *base_to)
{
	long	nbr_long;
	char	*str;
	int		len_t;
	int		len_f;

	len_t = 0;
	len_f = 0;
	while (base_from[len_f])
		len_f++;
	while (base_to[len_t])
		len_t++;
	if (detect_invalid(base_from, len_f) || detect_invalid(base_to, len_t))
		return (NULL);
	nbr_long = ft_atoi_base(nbr, base_from);
	str = itoa(nbr_long, int_size(nbr_long, base_to), base_to);
	return (str);
}
