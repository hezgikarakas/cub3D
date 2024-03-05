# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hakaraka <hakaraka@student.42vienna.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/03 21:39:25 by jkatzenb          #+#    #+#              #
#    Updated: 2024/03/05 10:10:47 by hakaraka         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D
SRCS = src/main.c src/error.c src/draw.c src/draw_textures.c src/draw_utils.c \
		src/parse_level.c src/parse_pass1.c src/parse_pass1_classify.c \
		src/parse_pass2.c src/map_checks.c src/util.c \
		src/controls.c src/textures.c
SRCS_BONUS = src/main.c src/error.c src/draw.c src/draw_textures.c src/draw_utils.c \
				src/parse_level.c src/parse_pass1.c src/parse_pass1_classify.c \
				src/parse_pass2.c src/map_checks.c src/util.c \
				src/controls_bonus.c src/textures.c
OBJS = $(SRCS:.c=.o)
OBJS_BONUS = $(SRCS_BONUS:.c=.o)
HDR = ./include/$(NAME).h
COMPILER = cc
RM = rm -f
LIBFT_PATH = ./include/libft_gio/
LIBFT = $(LIBFT_PATH)libft.a
#MLX_PATH = ./include/minilibx-linux/
#MLX = $(MLX_PATH)libmlx_Linux.a
CFLAGS = -Wall -Wextra -Werror -g #-I$(MLX_PATH)

all:	$(NAME)

$(NAME):	$(LIBFT) $(OBJS) $(HDR)
	@echo "\033[1;34m- linking executable: $(NAME)\033[0m"
	@$(COMPILER) $(CFLAGS) $(OBJS) $(LIBFT) -lXext -lX11 -lmlx -lm -o $@
	@echo "\033[1;32m- complete!\033[0m"

bonus:	$(LIBFT) $(OBJS_BONUS) $(HDR)
	@echo "\033[1;34m- linking executable: $(NAME)\033[0m"
	@$(COMPILER) $(CFLAGS) $(OBJS_BONUS) $(LIBFT) -lXext -lX11 -lmlx -lm -o $(NAME)
	@echo "\033[1;32m- complete!\033[0m"

%.o:	%.cpp
	@echo "\033[1;34m- compiling object: $<\033[0m"
	@$(COMPILER) $(CFLAGS) -o $@ -c $<

$(LIBFT):
	@make --no-print-directory all -C $(LIBFT_PATH)

#$(MLX):
#	@make --no-print-directory -C $(MLX_PATH)

clean:
	@make --no-print-directory clean -C $(LIBFT_PATH)
	@$(RM) $(OBJS) $(OBJS_BONUS)
	@echo "\033[1;31m- objects removed\033[0m"

fclean:	clean
	@make --no-print-directory fclean -C $(LIBFT_PATH)
	@$(RM) $(NAME)
	@echo "\033[1;31m- $(NAME) removed\033[0m"

re:	fclean all
#	@make --no-print-directory clean -C $(MLX_PATH)

#remlx:	fclean all

norm:
	norminette $(SRCS) $(HDR) controls_bonus.c

leakcheck:
	clear; valgrind --leak-check=full --track-origins=yes --track-fds=yes --show-reachable=yes --show-leak-kinds=all --error-limit=no -s ./cub3D maps/garden.cub

leakcheckerrors:
	clear ; \
	for f in \
		maps/empty_texture_name.cub maps/empty.cub maps/empty_line_in_map.cub \
		maps/multiple_players.cub maps/wrong_texture_ext.cub maps/wrong_texture_name.cub \
		maps/wrong_chars.cub maps/wrong_color.cub maps/one_color_doubled.cub maps/one_texture_doubled.cub \
		maps/texture_missing.cub maps/color_missing.cub maps/no_map.cub maps/no_args.cub maps/many_colors.cub \
		maps/garb1.cub maps/garb2.cub maps/luca_map.cub maps/tab.cub maps/few_colors.cub; do \
			echo "=== CHECKING $$f ===" ; \
			valgrind --leak-check=full --track-origins=yes --track-fds=yes --show-reachable=yes \
				--show-leak-kinds=all --error-limit=no -s ./cub3D $$f; \
	done

leakcheckgoodmaps:
	clear ; \
	for f in \
		maps/garden.cub maps/map1_for_debug.cub maps/map1.cub maps/map2.cub \
		maps/map3.cub maps/map4.cub maps/map5.cub maps/map6.cub maps/map7.cub; do \
			echo "=== CHECKING $$f ===" ; \
			valgrind --leak-check=full --track-origins=yes --track-fds=yes --show-reachable=yes \
				--show-leak-kinds=all --error-limit=no -s ./cub3D $$f; \
	done

.PHONY:	all clean fclean re norm leakcheck leakcheckerrors leakcheckgoodmaps
