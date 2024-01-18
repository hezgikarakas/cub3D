# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hakaraka <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/06 08:37:56 by hakaraka          #+#    #+#              #
#    Updated: 2022/12/06 08:38:00 by hakaraka         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= libftprintf.a
CC		= cc
CFLAGS		= -Wall -Wextra -Werror
SRCS		= ft_printf.c	ft_printwords.c	ft_printnumbers.c
OBJS		= $(SRCS:.c=.o)
RM		= rm -rf
AR		= ar -rcs

all     : $(NAME)

$(NAME) : $(OBJS)
	@echo "Linking $@"
	$(AR) $(NAME) $(OBJS)
	@echo "Done!"

%.o	: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean   :
	$(RM) $(OBJS)

fclean	: clean
	$(RM) $(NAME)

re	: fclean all

.PHONY  : all clean fclean re
