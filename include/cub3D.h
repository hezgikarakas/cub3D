/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karakasschueller <karakasschueller@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 12:47:02 by karakasschu       #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

// not yet # include "mlx.h"
# include "libft.h"
# include "get_next_line_bonus.h"
# include <stdlib.h>
# include <fcntl.h>

# define NUMBER_OF_TEXTURES 4
# define NUMBER_OF_COLORS 2

typedef struct s_player
{
	double	x;
	double	y;
	double	look_x;
	double	look_y;
}				t_player;

typedef struct		s_image
{
	char*	filename; // texture file name, needs to be freed on exit, is allocated by parse_level.c
	void*	img;
	int		width;
	int		height;
}					t_image;

typedef struct s_scene
{
	t_player	player;
	t_image		textures[NUMBER_OF_TEXTURES]; // currently: [north wall, east wall, south wall, west wall]
	int			colors[NUMBER_OF_COLORS][3]; // [floor, ceiling][red, green, blue]
	int			**map; // [y coordinate][x coordinate]
	int			map_width;
	int			map_height;
}				t_scene;

typedef struct	s_game
{
	void		*mlx;
	void		*mlx_win;
	t_scene		scene;
	char		*error_message; // no error if NULL
	char		*error_extramessage; // no extra info if NULL
}				t_game;

int			set_return_error(t_game *game, char *message);
int			set_return_error_extra(t_game *game, char *message, char *extramessage);
int			print_return_error(t_game *game);
int			interpret_arguments(int an, char **ac, t_game *game, char **map_fn);
int			init_scene(char *map_fn, t_game *game);
int			parse_level(char *map_fn, t_game *game);
int			interpret_arguments(int an, char **ac, t_game *game, char **map_fn);
int			parse_level(char *map_fn, t_game *game);

#endif