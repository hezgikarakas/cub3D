# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/03 21:39:25 by jkatzenb          #+#    #+#              #
#    Updated: 2024/02/21 15:49:30 by jkatzenb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D
SRCS = src/main.c src/error.c src/draw.c src/draw_textures.c src/draw_utils.c \
		src/parse_level.c src/parse_pass1.c src/parse_pass1_classify.c \
		src/parse_pass2.c src/map_checks.c src/util.c \
		src/controls.c src/textures.c
OBJS = $(SRCS:.c=.o)
HDR = ./include/$(NAME).h
COMPILER = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -f
LIBFT_PATH = ./include/libft_gio/
LIBFT = $(LIBFT_PATH)libft.a
MLX_PATH = ./include/minilibx-linux/
MLX = $(MLX_PATH)libmlx_Linux.a

all:	$(NAME)

$(NAME):	$(MLX) $(LIBFT) $(OBJS) $(HDR)
	@echo "\033[1;34m- linking executable: $(NAME)\033[0m"
	@$(COMPILER) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX) -lXext -lX11 -lm -o $@
	@echo "\033[1;32m- complete!\033[0m"

%.o:	%.cpp
	@echo "\033[1;34m- compiling object: $<\033[0m"
	@$(COMPILER) $(CFLAGS) -o $@ -c $<

$(LIBFT):
	@make --no-print-directory all -C $(LIBFT_PATH)

$(MLX):
	@make --no-print-directory -C $(MLX_PATH)

clean:
	@make --no-print-directory clean -C $(LIBFT_PATH)
	@$(RM) $(OBJS)
	@echo "\033[1;31m- objects removed\033[0m"

fclean:	clean
	@make --no-print-directory fclean -C $(LIBFT_PATH)
	@$(RM) $(NAME)
	@echo "\033[1;31m- $(NAME) removed\033[0m"

re:	fclean all
	@make --no-print-directory clean -C $(MLX_PATH)

remlx:	fclean all

norm:
	norminette $(SRCS) $(HDR)

leakcheck:
	clear; valgrind --leak-check=full --track-origins=yes --track-fds=yes --show-reachable=yes --show-leak-kinds=all --error-limit=no -s ./cub3D maps/test1.cub

.PHONY:	all clean fclean re norm leakcheck
