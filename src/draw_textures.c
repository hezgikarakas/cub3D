/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:35:07 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/02/08 16:36:42 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

static int	get_wall_direction(t_rc *rc)
{
	if (rc->side_hit && rc->raydir_y < 0)
		return (0);
	else if (!rc->side_hit && rc->raydir_x > 0)
		return (1);
	else if (rc->side_hit && rc->raydir_y > 0)
		return (2);
	else
		return (3);
}

static void	get_tex_x(t_rc *rc, double wall_x)
{
	rc->tex_x = (int)(wall_x * (double)64);
	if ((!rc->side_hit && rc->raydir_x < 0)
		|| (rc->side_hit && rc->raydir_y > 0))
		rc->tex_x = 64 - rc->tex_x - 1;
}

static double	get_wallx_texx_step(t_game *game, t_rc *rc)
{
	double	wall_x;

	if (!rc->side_hit)
		wall_x = game->player.pos_y + rc->perpwalldist * rc->raydir_y;
	else
		wall_x = game->player.pos_x + rc->perpwalldist * rc->raydir_x;
	get_tex_x(rc, wall_x - floor(wall_x));
	return (1.0 * (double)64 / (double)rc->line_height);
}

static void	distancefade(int x, int y, int colour, t_game *game)
{
	int	fadedist;

	if (game->scene.map.map_height < game->scene.map.map_width)
		fadedist = game->scene.map.map_height;
	else
		fadedist = game->scene.map.map_width;
	if (game->rc.side_hit == 0)
		colour = gradient_increment(colour, 0x000000, 7, 2);
	if (game->rc.perpwalldist > 3)
		colour = gradient_increment(colour, game->scene.fog
			, fadedist, game->rc.perpwalldist - 3);
	img_pixel_put(&game->img, x, y, colour);
}

void	draw_textures(t_game *game, t_rc *rc, int *draw_start_end, int x)
{
	double	step;
	double	tex_pos;
	int		colour;
	int		y;

	rc->walldir = get_wall_direction(rc);
	step = get_wallx_texx_step(game, rc);
	tex_pos = (draw_start_end[0] - WINDOW_HEIGHT / 2
			+ rc->line_height / 2) * step;
	y = draw_start_end[0];
	while (y <= draw_start_end[1])
	{
		rc->tex_y = (int)tex_pos & 63;
		tex_pos += step;
		colour = *(int *)(game->scene.textures[rc->walldir].texture_addr
				+ (rc->tex_y * game->scene.textures[rc->walldir].line_len
					+ rc->tex_x * (game->scene.textures[rc->walldir].bpp / 8)));
		distancefade(x, y, colour, game);
		y++;
	}
}
