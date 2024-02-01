/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:38:53 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/01/31 16:15:18 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

//	draws sky and floor with slight gradient
static void	draw_background(t_game *game)
{
	int	x;
	int	y;
	int	floor_colour2;
	int	sky_colour2;

	// floor_colour2 = gradient_increment(game->scene.colours[1][0], 0xaaaaaa, 4, 4);
	// sky_colour2 = gradient_increment(game->scene.colors[0][0],0xaaaaaa, 4, 4);
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

/*	perform dda
	and return perpendicular wall distance	*/
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
	if (!rc->side_hit)
		rc->perpwalldist = (rc->side_dist_x - rc->delta_dist_x);
	else
		rc->perpwalldist = (rc->side_dist_y - rc->delta_dist_y);
	return ((int)(WINDOW_HEIGHT / rc->perpwalldist));
}

/*	draws walls using raycasting
	draw_start_end are the lowest [0] 
	and highest [1] pixel of the current vertical stripe	*/
void	draw_objects(t_game *game, t_rc *rc)
{
	int	x;
	int	draw_start_end[2];

	x = 0;
	while (x < WINDOW_WIDTH)
	{
		init_rc(rc, game, x);
		step_dist(rc, game);
		rc->line_height = dda_algorithm(rc, game);
		draw_start_end[0] = -rc->line_height / 2 + WINDOW_HEIGHT / 2;
		if (draw_start_end[0] < 0)
			draw_start_end[0] = 0;
		draw_start_end[1] = rc->line_height / 2 + WINDOW_HEIGHT / 2;
		if (draw_start_end[1] >= WINDOW_HEIGHT)
			draw_start_end[1] = WINDOW_HEIGHT -1;
		// colour = DEFAULT_WALL;
		// if (rc->side_hit == 1)
		// 	colour = gradient_increment(colour, 0x031721, 7, 1);
		// ver_line(game, x, draw_start_end, colour);
		draw_textures(game, rc, draw_start_end, x);
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
	// game->img.mlx_img = mlx_new_image(game->ptrs.mlx, WINDOW_WIDTH,
	// 		WINDOW_HEIGHT);
	game->img.addr = mlx_get_data_addr(game->img.mlx_img, &game->img.bpp,
			&game->img.line_len, &game->img.endian);
	draw_background(game);
	draw_objects(game, game->rc);
	mlx_put_image_to_window(game->ptrs.mlx, game->ptrs.win,
		game->img.mlx_img, 0, 0);
	// mlx_destroy_image(game->ptrs.mlx, game->img.mlx_img);
	draw_ui(game);
	return (0);
}
