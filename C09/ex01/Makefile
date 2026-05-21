# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/21 11:28:52 by kpiwan            #+#    #+#              #
#    Updated: 2026/05/21 14:45:13 by kpiwan           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CFLAGS = -Wall -Werror -Wextra
CC = cc

SRCS = srcs/ft_swap.c srcs/ft_putstr.c srcs/ft_putchar.c srcs/ft_strcmp.c srcs/ft_strlen.c
DEST = $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I includes -c $< -o $@

$(NAME): $(DEST)
	ar rcs $(NAME) $(DEST)

clean:
	rm $(DEST)

fclean: clean
	rm $(NAME)

re: fclean all

.PHONY : all clean fclean re

