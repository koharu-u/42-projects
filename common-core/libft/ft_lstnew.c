/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 19:26:27 by kpiwan            #+#    #+#             */
/*   Updated: 2026/09/04 19:48:01 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*t_content;

	t_content = malloc(sizeof(*t_content));
	if (!t_content)
		return (NULL);
	t_content->content = content;
	t_content->next = NULL;
	return (t_content);
}
