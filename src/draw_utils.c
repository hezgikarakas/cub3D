/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:42:50 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/01/31 16:08:25 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

//	draws pixels to image
void	img_pixel_put(t_img *img, int x, int y, int color)
{
	char	*pixel;

	if (((x > 0) && (y > 0)) && ((x < WINDOW_WIDTH) && (y < WINDOW_HEIGHT)))
	{
		pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
		*(int *)pixel = color;
	}
}

//	returns color gradient increment
int	gradient_increment(int start, int end, int stepc, int stepn)
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

/*	calculate position and rotation of camera plane,
	get deltas for dda and init vars*/
void	init_rc(t_rc *rc, t_game *game, int x)
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

//	draws verticak lines
void	ver_line(t_game *game, int x, int *strt_end, int colour)
{
	int	y;

	y = strt_end[0];
	while (y <= strt_end[1])
	{
		img_pixel_put(&game->img, x, y, colour);
		y++;
	}
}
