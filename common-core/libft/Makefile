# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kpiwan <kpiwan@student.42bangkok.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/29 15:29:59 by kpiwan            #+#    #+#              #
#    Updated: 2026/08/30 12:37:46 by kpiwan           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CFLAGS = -Wall -Werror -Wextra
CC = cc

SRCS =	ft_swap.c 
				ft_putstr.c  \
				ft_putchar.c \
				ft_strcmp.c  \
				ft_strlen.c  \
				ft_atoi.c   \
				ft_calloc.c   \
				ft_isalpha.c  \
				ft_isdigit.c  \
				ft_itoa.c    \
				ft_putchar_fd.c  \
				ft_putnbr_fd.c  \
				ft_strchr.c   \
				ft_strlcpy.c  \
				ft_strncmp.c  \
				ft_tolower.c  \
				ft_bzero.c  \
				ft_isalnum.c  \
				ft_isascii.c  \
				ft_isprint.c  \
				ft_memset.c  \
				ft_putendl_fd.c  \
				ft_putstr_fd.c  \
				ft_strlcat.c  \
				ft_strlen.c   \
				ft_strrchr.c  \
				ft_toupper.c  \
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

