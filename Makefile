# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jkatzenb <jkatzenb@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/03 21:39:25 by jkatzenb          #+#    #+#              #
#    Updated: 2024/03/20 13:47:43 by jkatzenb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D
BONUS_NAME = cub3D_bonus
COMPILER = cc
RM = rm -f

SDIR = ./src/
SFILES =	main.c\
			error.c\
			draw.c\
			draw_textures.c\
			draw_utils.c\
			parse_level.c\
			parse_pass1.c\
			parse_pass1_classify.c\
			parse_pass2.c\
			map_checks.c\
			util.c\
			controls.c\
			textures.c
BONUS_SFILES = $(SFILES:.c=_bonus.c)
SRCS = $(addprefix ${SDIR}, ${SFILES})
BONUS_SRCS = $(addprefix ${SDIR}, ${BONUS_SFILES})

ODIR = ./obj/
OFILES = $(SFILES:.c=.o)
OBJS = $(addprefix ${ODIR}, ${OFILES})
BONUS_OFILES = $(BONUS_SFILES:.c=.o)
BONUS_OBJS = $(addprefix ${ODIR}, ${BONUS_OFILES})

IDIR = ./include/

LIBFT_PATH = $(IDIR)libft_gio/
LIBFT = $(LIBFT_PATH)libft.a

HDR = $(IDIR)$(NAME).h

CFLAGS = -Wall -Wextra -Werror
INCLUDE = -I $(IDIR) -I $(LIBFT_PATH)
DEBUG = -g
LIBRARIES = -lXext -lX11 -lmlx -lm

STD = \033[0m
RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
PINK = \033[1;35m
CYAN = \033[1;36m
CREAM = \033[1;37m

all:	$(NAME)

$(NAME):	$(LIBFT) $(ODIR) $(OBJS) $(HDR)
	@echo "$(CYAN)- linking executable: $(NAME)$(STD)"
	@$(COMPILER) $(CFLAGS) $(INCLUDE) $(DEBUG) $(OBJS) $(LIBFT) $(LIBRARIES) -o $@
	@echo "$(GREEN)- complete!$(STD)"

$(BONUS_NAME):	$(LIBFT) $(ODIR) $(BONUS_OBJS) $(HDR)
	@echo "$(CYAN)- linking executable: $(NAME)_$@$(STD)"
	@$(COMPILER) $(CFLAGS) $(INCLUDE) $(DEBUG) $(BONUS_OBJS) $(LIBFT) $(LIBRARIES) -o $(BONUS_NAME)
	@echo "$(GREEN)- complete!$(STD)"

$(ODIR)%.o:	$(SDIR)%.c
	@echo "$(BLUE)- compiling object from: $<$(STD)"
	@$(COMPILER) $(CFLAGS) $(INCLUDE) $(DEBUG) -o $@ -c $<

$(ODIR):
	@echo "$(PINK)- creating obj directory$(STD)"
	@mkdir -p $(ODIR)

$(LIBFT):
	@make --no-print-directory all -C $(LIBFT_PATH)

clean:
	@make --no-print-directory clean -C $(LIBFT_PATH)
	@if [ -d "$(ODIR)" ]; then \
		$(RM) -r $(ODIR); echo "$(RED)- objects removed$(STD)"; \
	fi

fclean:	clean
	@make --no-print-directory fclean -C $(LIBFT_PATH)
	@if [ -f "$(NAME)" ]; then \
		$(RM) $(NAME); echo "$(RED)- $(NAME) removed$(STD)"; \
	fi
	@if [ -f "$(BONUS_NAME)" ]; then \
		$(RM) $(BONUS_NAME); echo "$(RED)- $(BONUS_NAME) removed$(STD)"; \
	fi

re:	fclean all

norm:
	norminette $(SRCS) $(BONUS_SRCS) $(HDR)

leakcheck:
	clear; valgrind --leak-check=full --track-origins=yes --track-fds=yes --show-reachable=yes --show-leak-kinds=all --error-limit=no -s ./cub3D maps/garden.cub

leakcheckerrors:
	clear ; \
	for f in \
		maps/garb2.cub maps/luca_map.cub maps/tab.cub maps/few_colors.cub maps/wrong_color2.cub \
		maps/empty_texture_name.cub maps/empty.cub maps/empty_line_in_map.cub maps/texture_wrongline.cub \
		maps/multiple_players.cub maps/wrong_texture_ext.cub maps/wrong_texture_name.cub maps\wrong_chars2.cub \
		maps/wrong_chars.cub maps/wrong_color.cub maps/one_color_doubled.cub maps/one_texture_doubled.cub \
		maps/texture_missing.cub maps/color_missing.cub maps/no_map.cub maps/no_args.cub maps/many_colors.cub \
		maps/garb1.cub maps/wrong_color3.cub; do \
			echo "$(YELLOW)=== CHECKING $$f ===$(STD)" ; \
			valgrind --leak-check=full --track-origins=yes --track-fds=yes --show-reachable=yes \
				--show-leak-kinds=all --error-limit=no -s ./cub3D $$f; \
	done

leakcheckgoodmaps:
	clear ; \
	for f in \
		maps/garden.cub maps/map1_for_debug.cub maps/map1.cub maps/map2.cub \
		maps/map3.cub maps/map4.cub maps/map5.cub maps/map6.cub maps/map7.cub; do \
			echo "$(YELLOW)=== CHECKING $$f ===$(STD)" ; \
			valgrind --leak-check=full --track-origins=yes --track-fds=yes --show-reachable=yes \
				--show-leak-kinds=all --error-limit=no -s ./cub3D $$f; \
	done

.PHONY:	all clean fclean re norm leakcheck leakcheckerrors leakcheckgoodmaps
