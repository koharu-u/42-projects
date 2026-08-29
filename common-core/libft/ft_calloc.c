/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 18:27:39 by kpiwan            #+#    #+#             */
/*   Updated: 2026/08/29 18:41:05 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	*calloc(size_t nmemb, size_t size);

void	*calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	total;

	if (size != 0 && nmemb > (size_t)-1 / size)
		return (NULL);
	total = nmemb * size;
	ptr = malloc(total) if (!ptr) return (NULL);
	ft_bzero(ptr, total);
	return (ptr);
}
