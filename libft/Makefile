# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hakaraka <hakaraka@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/07 10:14:17 by hakaraka          #+#    #+#              #
#    Updated: 2023/08/02 10:27:48 by hakaraka         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	libft.a
CC				=	cc
CFLAGS			=	-Wall -Wextra -Werror

SRCS			= 	ft_isalpha.c	ft_isdigit.c	ft_isalnum.c \
				ft_isascii.c	ft_isprint.c	ft_strlen.c \
				ft_memset.c	ft_bzero.c	ft_memcpy.c \
				ft_memmove.c	ft_strlcpy.c	ft_strlcat.c \
				ft_toupper.c	ft_tolower.c	ft_strchr.c \
				ft_strrchr.c	ft_strncmp.c	ft_memchr.c \
				ft_memcmp.c	ft_strnstr.c	ft_atoi.c \
				ft_calloc.c	ft_strdup.c \
				ft_substr.c	ft_strjoin.c	ft_strtrim.c \
				ft_split.c	ft_itoa.c	ft_strmapi.c \
				ft_striteri.c	ft_putchar_fd.c	ft_putstr_fd.c \
				ft_putendl_fd.c	ft_putnbr_fd.c

OBJS			= $(SRCS:%.c=%.o)

EXTRA			= get_next_line_bonus.c get_next_line_utils_bonus.c

EXTRA_OBJS		= $(EXTRA:.c=.o)

.PHONY:			all clean fclean re extra

all:			$(NAME)

$(NAME):		$(OBJS)
			@echo "Linking $@"
			@ar rcs $(NAME) $(OBJS)
			@echo "Done!"

clean:
			rm -rf $(OBJS) $(EXTRA_OBJS)

fclean: 		clean
			rm -rf $(NAME)

re:				fclean all

extra:			$(OBJS) $(EXTRA_OBJS)
			@echo "Linking with get_next_line!"
			ar rcs $(NAME) $(OBJS) $(EXTRA_OBJS)

