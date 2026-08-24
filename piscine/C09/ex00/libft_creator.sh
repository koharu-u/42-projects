# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    libft_creator.sh                                   :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/21 11:11:23 by kpiwan            #+#    #+#              #
#    Updated: 2026/05/21 11:11:25 by kpiwan           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/bin/sh

cc -Wall -Werror -Wextra -c ft_putstr.c ft_putchar.c ft_strcmp.c ft_strlen.c ft_swap.c
ar rcs libft.a ft_putstr.o ft_putchar.o ft_strcmp.o ft_strlen.o ft_swap.o
