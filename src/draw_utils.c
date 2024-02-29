/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaraka <hakaraka@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:42:50 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/02/29 13:17:54 by hakaraka         ###   ########.fr       */
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
int	gradient_increment(int start, int end, int step_c, float step_n)
{
	float	step_r;
	float	step_g;
	float	step_b;

	if (step_n > step_c)
		step_n = step_c;
	step_r = (float)(((end >> 16) & 0xFF)
			- ((start >> 16) & 0xFF)) / (float)step_c;
	step_g = (float)(((end >> 8) & 0xFF)
			- ((start >> 8) & 0xFF)) / (float)step_c;
	step_b = (float)((end & 0xFF) - (start & 0xFF)) / (float)step_c;
	return (((int)(((start >> 16) & 0xFF) + step_n * step_r) << 16)
			+ ((int)(((start >> 8) & 0xFF) + step_n * step_g) << 8)
			+ ((int)(((start >> 0) & 0xFF) + step_n * step_b) << 0));
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
	if (rc->raydir_x != 0)
		rc->delta_dist_x = fabs(1.0 / rc->raydir_x);
	else
		rc->delta_dist_x = __DBL_MAX__;
	if (rc->raydir_y != 0)
		rc->delta_dist_y = fabs(1.0 / rc->raydir_y);
	else
		rc->delta_dist_y = __DBL_MAX__;
	rc->wall_hit = 0;
	rc->perpwalldist = 0;
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

int	outofbounds(t_rc *rc, t_game *game)
{
	if ((rc->map_x < 0 || rc->map_x > game->scene.map.map_width - 1)
		|| (rc->map_y < 0 || rc->map_y > game->scene.map.map_height - 1))
	{
		rc->perpwalldist = __DBL_MAX__;
		return (1);
	}
	return (0);
}
