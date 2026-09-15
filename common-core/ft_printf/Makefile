# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/29 15:29:59 by kpiwan            #+#    #+#              #
#    Updated: 2026/09/13 08:47:22 by kpiwan           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a

CFLAGS = -Wall -Werror -Wextra
CC = cc

SRCS = ""
DEST = $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c libft.h
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(DEST)
	ar rcs $(NAME) $(DEST)

clean:
	rm -f $(DEST)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY : all clean fclean re
