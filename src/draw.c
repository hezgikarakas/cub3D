/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:38:53 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/01/25 15:42:41 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

//	draws pixels to image
static void	img_pixel_put(t_img *img, int x, int y, int color)
{
	char	*pixel;

	if (((x > 0) && (y > 0)) && ((x < WINDOW_WIDTH) && (y < WINDOW_HEIGHT)))
	{
		pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
		*(int *)pixel = color;
	}
}

//	returns color gradient increment
static int	gradient_increment(int start, int end, int stepc, int stepn)
{
	float	step_r;
	float	step_g;
	float	step_b;

	step_r = (float)(((end >> 16) & 0xFF)
			- ((start >> 16) & 0xFF)) / (float)stepc;
	step_g = (float)(((end >> 8) & 0xFF)
			- ((start >> 8) & 0xFF)) / (float)stepc;
	step_b = (float)((end & 0xFF) - (start & 0xFF)) / (float)stepc;
	return (((int)(((start >> 16) & 0xFF) + stepn * step_r) << 16)
			+ ((int)(((start >> 8) & 0xFF) + stepn * step_g) << 8)
			+ ((int)(((start >> 0) & 0xFF) + stepn * step_b) << 0));
}

//	draws sky and floor with slight gradient
static void	draw_background(t_game *game)
{
	int	x;
	int	y;
	int	floor_colour2;
	int	sky_colour2;

	// floor_colour2 = gradient_increment(game->scene.colours[1][0], 0xaaaaaa, 4, 4);
	// sky_colour2 = gradient_increment(game->scene.colours[0][0],0xaaaaaa, 4, 4);
	sky_colour2 = DEFAULT_SKY_GRADIENT;
	floor_colour2 = DEFAULT_FLOOR_GRADIENT;
	y = 0;
	while (y < WINDOW_HEIGHT)
	{
		x = 0;
		while (x < WINDOW_WIDTH)
		{
			if (y < WINDOW_HEIGHT / 2)
				img_pixel_put(&game->img, x, y,
					gradient_increment(game->scene.ceiling_colour,
						sky_colour2, WINDOW_HEIGHT / 2, y));
			else
				img_pixel_put(&game->img, x, y,
					gradient_increment(floor_colour2, game->scene.floor_colour,
						WINDOW_HEIGHT / 2, y - WINDOW_HEIGHT / 2));
			x++;
		}
		y++;
	}
}

//	draws vertical lines
static void	ver_line(t_game *game, int x, int *strt_end, int colour)
{
	int	y;

	y = strt_end[0];
	while (y <= strt_end[1])
	{
		img_pixel_put(&game->img, x, y, colour);
		y++;
	}
}

/*	calculate position and rotation of camera plane,
	get deltas for dda and init vars*/
static void	init_rc(t_rc *rc, t_game *game, int x)
{
	rc->camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
	rc->raydir_x = game->player.look_x + game->player.plane_x * rc->camera_x;
	rc->raydir_y = game->player.look_y + game->player.plane_y * rc->camera_x;
	rc->map_x = (int)game->player.pos_x;
	rc->map_y = (int)game->player.pos_y;
	rc->delta_dist_x = fabs(1 / rc->raydir_x);
	rc->delta_dist_y = fabs(1 / rc->raydir_y);
	rc->wall_hit = 0;
}

//	calc step and initial sidedist
static void	step_dist(t_rc *rc, t_game *game)
{
	if (rc->raydir_x < 0)
	{
		rc->step_x = -1;
		rc->side_dist_x = (game->player.pos_x - rc->map_x) * rc->delta_dist_x;
	}
	else
	{
		rc->step_x = 1;
		rc->side_dist_x = (rc->map_x + 1.0
				- game->player.pos_x) * rc->delta_dist_x;
	}
	if (rc->raydir_y < 0)
	{
		rc->step_y = -1;
		rc->side_dist_y = (game->player.pos_y - rc->map_y) * rc->delta_dist_y;
	}
	else
	{
		rc->step_y = 1;
		rc->side_dist_y = (rc->map_y + 1.0
				- game->player.pos_y) * rc->delta_dist_y;
	}
}

/*	perform dda, calculate distance projected on camera direction
	and return height of line to draw on screen	*/
static int	dda_algorithm(t_rc *rc, t_game *game)
{
	while (rc->wall_hit == 0)
	{
		if (rc->side_dist_x < rc->side_dist_y)
		{
			rc->side_dist_x += rc->delta_dist_x;
			rc->map_x += rc->step_x;
			rc->side_hit = 0;
		}
		else
		{
			rc->side_dist_y += rc->delta_dist_y;
			rc->map_y += rc->step_y;
			rc->side_hit = 1;
		}
		if (game->scene.map.map[rc->map_y][rc->map_x] > 0)
			rc->wall_hit = 1;
	}
	if (rc->side_hit == 0)
		rc->perpwalldist = (rc->side_dist_x - rc->delta_dist_x);
	else
		rc->perpwalldist = (rc->side_dist_y - rc->delta_dist_y);
	return ((int)(WINDOW_HEIGHT / rc->perpwalldist));
}

//	draws walls using raycasting
void	draw_objects(t_game *game, t_rc *rc)
{
	int	x;
	int	line_height;
	int	colour;
	int	draw_start_end[2];

	x = 0;
	while (x < WINDOW_WIDTH)
	{
		init_rc(rc, game, x);
		step_dist(rc, game);
		line_height = dda_algorithm(rc, game);
		//calculate lowest and highest pixel to fill in current stripe
		draw_start_end[0] = -line_height / 2 + WINDOW_HEIGHT / 2;
		if (draw_start_end[0] < 0)
			draw_start_end[0] = 0;
		draw_start_end[1] = line_height / 2 + WINDOW_HEIGHT / 2;
		if (draw_start_end[1] >= WINDOW_HEIGHT)
			draw_start_end[1] = WINDOW_HEIGHT -1;
		colour = DEFAULT_WALL;
		// if (rc->side_hit == 1)	colour = gradient_increment(colour, gradient_increment(colour, 0x031721, 7, 1), 3, 3);
		if (rc->side_hit == 1)
			colour = gradient_increment(colour, 0x031721, 7, 1);
		ver_line(game, x, draw_start_end, colour);
		x++;
	}
}

//this function is mainly for testing, leaks and uses not allowed funtions,to be removed later
void	draw_ui(t_game *game)
{
	char	dirx[30];
	char	diry[30];
	char	posx[30];
	char	posy[30];
	char	move[30];
	char	rot[30];
	sprintf(dirx, "look x: %f", game->player.look_x);
	sprintf(diry, "look y: %f", game->player.look_y);
	sprintf(posx, "pos x: %f", game->player.pos_x);
	sprintf(posy, "pos y: %f", game->player.pos_y);
	sprintf(move, "movement speed: %f", game->player.movespeed);
	sprintf(rot, "rotate speed: %f", game->player.rotspeed);
	
	mlx_string_put(game->ptrs.mlx, game->ptrs.win, 10, 15, 0x000000, dirx);
	mlx_string_put(game->ptrs.mlx, game->ptrs.win, 10, 25, 0x000000, diry);
	mlx_string_put(game->ptrs.mlx, game->ptrs.win, 10, 45, 0x000000, posx);
	mlx_string_put(game->ptrs.mlx, game->ptrs.win, 10, 55, 0x000000, posy);
	mlx_string_put(game->ptrs.mlx, game->ptrs.win, 10, WINDOW_HEIGHT - 15, 0x000000, move);
	mlx_string_put(game->ptrs.mlx, game->ptrs.win, 10, WINDOW_HEIGHT - 25, 0x000000, rot);
}

//rendering loop
int	render(t_game *game)
{
	game->img.mlx_img = mlx_new_image(game->ptrs.mlx, WINDOW_WIDTH,
			WINDOW_HEIGHT);
	game->img.addr = mlx_get_data_addr(game->img.mlx_img, &game->img.bpp,
			&game->img.line_len, &game->img.endian);
	draw_background(game);
	draw_objects(game, game->rc);
	mlx_put_image_to_window(game->ptrs.mlx, game->ptrs.win,
		game->img.mlx_img, 0, 0);
	mlx_destroy_image(game->ptrs.mlx, game->img.mlx_img);
	draw_ui(game);
	return (0);
}
