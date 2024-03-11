# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/11 18:28:22 by jkatzenb          #+#    #+#              #
#    Updated: 2024/03/11 19:19:06 by jkatzenb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
COMPILER = cc
RM = rm -f

MAIN_SRCS = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
		ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
		ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
		ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
		ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c \
		ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
		ft_itoa.c ft_strmapi.c ft_striteri.c ft_strtrim.c ft_split.c

BONUS_SRCS = ft_lstnew.c ft_lstadd_front.c ft_lstadd_back.c ft_lstsize.c \
		ft_lstlast.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c

PRINTF_SRCS = ft_printf.c printf_check.c printf_c.c printf_s.c printf_d_i.c \
		printf_u.c printf_x.c printf_p.c ft_uitoa.c
		
GNL_SRCS = get_next_line.c

FPRINTF = ft_fprintf.c fprintf_check.c fprintf_c.c fprintf_s.c fprintf_d_i.c \
		fprintf_u.c fprintf_x.c fprintf_p.c

XTRA_SRCS = ft_atol.c

SFILES = $(MAIN_SRCS) $(BONUS_SRCS) $(PRINTF_SRCS) $(GNL_SRCS) $(FPRINTF) $(XTRA_SRCS)
SDIR = ./libft_src/
SRCS = $(addprefix $(SDIR), $(SFILES))

ODIR = ./libft_obj/
OFILES = $(SFILES:.c=.o)
OBJS = $(addprefix $(ODIR), $(OFILES))

HDR = libft.h

CFLAGS = -Wall -Wextra -Werror
INCLUDE = -I .
DEBUG = -g

STD = \033[0m
RED = \033[3;31m
GREEN = \033[3;32m
YELLOW = \033[3;33m
BLUE = \033[3;34m
PINK = \033[3;35m
CYAN = \033[3;36m
CREAM = \033[3;37m

all:	$(NAME)

$(NAME):	$(ODIR) $(OBJS) $(HDR)
	@echo -e "$(CYAN)- linking $@$(STD)"
	@ar crs $@ $(OBJS)
	@echo -e "$(GREEN)- libft complete!$(STD)"

$(ODIR)%.o:	$(SDIR)%.c
	@echo -e "$(BLUE)- compiling libft object: $@$(STD)"
	@$(COMPILER) $(CFLAGS) $(INCLUDE) $(DEBUG) -o $@ -c $<

$(ODIR):
	@echo -e "$(PINK)- creating libft_obj directory$(STD)"
	@mkdir -p $(ODIR)

clean:
	@if [ -d "$(ODIR)" ]; then \
		$(RM) -r $(ODIR); echo -e "$(RED)- libft objects removed$(STD)"; \
	fi

fclean:	clean
	@if [ -f "$(NAME)" ]; then \
		$(RM) $(NAME); echo -e "$(RED)- $(NAME) removed$(STD)"; \
	fi

re:	fclean all

.PHONY:	all clean fclean re