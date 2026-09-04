/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 20:28:48 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/04 21:36:50 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*lst_last;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	lst_last = ft_lstlast(*lst);
	lst_last->next = new;
}
