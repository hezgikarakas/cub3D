# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hakaraka <hakaraka@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/19 10:21:52 by hakaraka          #+#    #+#              #
#    Updated: 2024/01/19 10:28:42 by hakaraka         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D

SRCS =  src/main.c

OBJS = ${SRCS:.c=.o}

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes

RM = rm -rf

all: ${NAME}
${NAME}: ${OBJS}
	${MAKE} -C ./libft extra
	${CC} ${CFLAGS} ${OBJS} ./libft/libft.a -o ${NAME}

clean: 
	@${MAKE} -C ./libft fclean
	@${RM} ${OBJS}

fclean: clean
	@${RM} ./libft/libft.a
	@${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
