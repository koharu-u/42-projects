# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/29 15:29:59 by kpiwan            #+#    #+#              #
#    Updated: 2026/08/29 15:30:27 by kpiwan           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CFLAGS = -Wall -Werror -Wextra
CC = cc

SRCS = ft_swap.c ft_putstr.c ft_putchar.c ft_strcmp.c ft_strlen.c
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

