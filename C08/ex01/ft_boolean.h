/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_boolean.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 18:33:15 by kpiwan            #+#    #+#             */
/*   Updated: 2026/05/19 18:45:23 by kpiwan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_BOOLEAN_H
# define FT_BOOLEAN_H

# include <unistd.h>
# define TRUE 1
# define FALSE 0
# define SUCCESS 0
# define ODD_MSG "have an odd number of arguments."
# define EVEN_MSG "have an even number of arguments."

# define EVEN(nbr) ((nbr % 2))

typedef int	t_bool;

#endif
