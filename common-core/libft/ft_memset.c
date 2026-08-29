/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 18:26:32 by kpiwan            #+#    #+#             */
/*   Updated: 2026/08/26 18:34:01 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	*ft_bzero(void *b, int c, unsigned long n)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)b;
	while (n--)
	{
		*ptr = (unsigned char)c;
		ptr++;
	}
	return (b);
}
