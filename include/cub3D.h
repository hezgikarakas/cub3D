/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 12:47:02 by karakasschu       #+#    #+#             */
/*   Updated: 2024/01/31 16:08:39 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "./minilibx-linux/mlx.h"
# include "./libft_gio/libft.h"
//	# include "./libft/get_next_line_bonus.h"
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <math.h>
# include <X11/keysym.h>

//	screen resolution
# define WINDOW_WIDTH 640
# define WINDOW_HEIGHT 480

//	default values
# define DEFAULT_FLOOR 0x57f292
# define DEFAULT_FLOOR_GRADIENT 0x07775f
# define DEFAULT_SKY 0x5f61e8
# define DEFAULT_SKY_GRADIENT 0xf25792
# define DEFAULT_WALL 0xbebeee
# define DEFAULT_DISTANCE_FADE 0x2a564d

# define NUMBER_OF_TEXTURES 4

//	contains mlx and window pointers
typedef struct	s_ptrs
{
	void *mlx;
	void *win;
}				t_ptrs;

//	contains everything neccessary to draw into image
typedef struct	s_img
{
	void	*mlx_img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}				t_img;

//	contains all variables pertaining to the player
typedef struct	s_player
{
	double	pos_x;
	double	pos_y;
	double	look_x;
	double	look_y;
	double	plane_x;
	double	plane_y;
	double	movespeed;
	double	rotspeed;
}				t_player;

//	contains all variables for textures
//	filename needs to be freed on exit, is allocated by parse_level.c
typedef struct	s_texture
{
	char*	filename;
	void*	texture_img;
	char*	texture_addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		width;
	int		height;
	int		*texture;
}				t_texture;

//	contains everything to do with the map
//	map_name is not dynamically allocated, it is taken from command line!
//	map[y coordinate][x coordinate]
typedef struct	s_map
{
	char	*map_name;
	int		**map;
	int		map_width;
	int		map_height;
}				t_map;

//	contains everything that exists in the game world
//	textures[north wall, east wall, south wall, west wall]
//	color is encoded as 0xRRGGBB in one integer
typedef struct	s_scene
{
	t_map		map;
	t_texture	textures[NUMBER_OF_TEXTURES];
	int			ceiling_colour;
	int			floor_colour;
}				t_scene;

//	contains all variables used in raycasting
typedef struct	s_rc
{
	double	camera_x;
	double	raydir_y;
	double	raydir_x;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	perpwalldist;
	int		map_x;
	int		map_y;
	int		step_x;
	int		step_y;
	int		wall_hit;
	int		side_hit;
	int		line_height;
	int		walldir;
	int		tex_x;
	int		tex_y;
}				t_rc;

//	contains everything that exists outside the game world
typedef struct	s_game
{
	t_ptrs		ptrs;
	t_img		img;
	t_scene		scene;
	t_player	player;
	t_rc		rc;
}				t_game;

// helper structure for pass 1 of parsing
typedef struct s_parse_helper
{
	int			map_fd;
	char		*line_temp;
	int			linelength;
	t_game		*game;
	int			found_floor_color;
	int			found_ceiling_color;
	int			found_map_start;
	int			found_map_end;
	int			line_idx;
	int			map_start_idx;
	int			map_end_idx;
	int			map_max_line_length;
	int			interpreted_this_line;
} t_parse_helper;

//	draw.c
int		render(t_game *game);
//	draw_textures.c
void	draw_textures(t_game *game, t_rc *rc, int *draw_start_end, int x);
//	draw_utils.c
void	img_pixel_put(t_img *img, int x, int y, int color);
void	init_rc(t_rc *rc, t_game *game, int x);
void	ver_line(t_game *game, int x, int *strt_end, int colour);
int		gradient_increment(int start, int end, int stepc, float stepn);
//	textures.c
void	load_texture(t_game *game);
//	controls.c
int		handle_keypress(int keysym, t_game *game);
int		close_window(t_game *game);
//	error.c
int		error_return(int type, char *error_message, int error_code);
int		error_return_s(int type, char *error_message, int error_code, char *extra);
//	parse_level.c
int	process_arguments(int ac, char **av, t_game *game);
int	parse_level(char *map_fn, t_game *game);
int	**allocate_map(int rows, int cols); //this function used to be static, i changed it to be able to use it in my temporary bypass function,
										//should be changed back to static once it is no longer needed there
// parse_pass1.c
int parse_mapfile_pass_1(char *map_fn, t_game *game, int *map_start_line);
// parse_pass1_classify.c
int pass1_classify_trimmed_line(t_parse_helper *ph, char *line_temp, char *s);
// parse_pass2.c
int parse_mapfile_pass_2(char *map_fn, t_game *game, int start_idx);
// map_checks.c
int map_final_checks(t_game* game);

#endif