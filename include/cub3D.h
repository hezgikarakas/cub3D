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
// # define DEFAULT_FLOOR 0x37dd08
# define DEFAULT_FLOOR 0x57f292
# define DEFAULT_FLOOR_GRADIENT 0x07775f
// # define DEFAULT_SKY 0x07c2ed
# define DEFAULT_SKY 0x5f61e8
# define DEFAULT_SKY_GRADIENT 0xf25792
# define DEFAULT_WALL 0xbebeee

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
// renamed this to avoid confusion with t_img
typedef struct	s_texture
{
	char*	filename; // texture file name, needs to be freed on exit, is allocated by parse_level.c
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
//	created this struct because having all map related stuff in its own struct felt more intuitive but im now questioning if this might have been a mistake
typedef struct	s_map
{
	char	*map_name;
	int		**map;// [y coordinate][x coordinate]
	int		map_width;
	int		map_height;
}				t_map;

//	contains everything that exists in the game world
typedef struct	s_scene
{
	t_map		map;
	t_texture	textures[NUMBER_OF_TEXTURES]; // currently: [north wall, east wall, south wall, west wall]
	// i think i would prefer having seperate ceiling and floor colour variables instead of one array
	// that way it is easier to see which is which at one glance
	// ok, and colour is 0xRRGGBB it seems, have changed that
	int			ceiling_colour;
	int			floor_colour;
}				t_scene;

//	contains all variables used in raycasting
typedef struct	s_rc
{
	double	camera_x;
	double	raydir_x;
	double	raydir_y;
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
	t_rc		*rc; // why pointer? why not use without * then we do not need malloc
	//if using the new error function i suggested these two variables would be redundand i think
	char		*error_message; // no error if NULL
	char		*error_extramessage; // no extra info if NULL
}				t_game;

//	draw.c
int		render(t_game *game);
//	draw_textures.c
void	draw_textures(t_game *game, t_rc *rc, int *draw_start_end, int x);
//	draw_utils.c
void	img_pixel_put(t_img *img, int x, int y, int color);
void	init_rc(t_rc *rc, t_game *game, int x);
void	ver_line(t_game *game, int x, int *strt_end, int colour);
int		gradient_increment(int start, int end, int stepc, int stepn);
//	textures.c
void	load_texture(t_game *game);
//	controls.c
int		handle_keypress(int keysym, t_game *game);
int		close_window(t_game *game);
//	error.c
int		error_return(int type, char *error_message, int error_code);
int		set_return_error(t_game *game, char *message);
int		set_return_error_extra(t_game *game, char *message, char *extramessage);
int		print_return_error(t_game *game);
//	parse_level.c
int	process_arguments(int ac, char **av, t_game *game);
int	parse_level(char *map_fn, t_game *game);
int	**allocate_map(int rows, int cols); //this function used to be static, i changed it to be able to use it in my temporary bypass function,
										//should be changed back to static once it is no longer needed there

#endif