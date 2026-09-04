/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 21:51:43 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/04 22:14:54 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*ptr;
	t_list	*ptr_del;

	ptr = *lst;
	while (ptr != NULL)
	{
		ptr_del = ptr;
		ptr = ptr->next;
		ft_lstdelone(ptr_del, del);
	}
	*lst = NULL;
}
