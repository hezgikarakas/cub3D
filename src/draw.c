/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:38:53 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/01/22 06:29:04 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

//draws pixels to image
static void	img_pixel_put(t_img *img, int x, int y, int color)
{
	char	*pixel;

	if (((x > 0) && (y > 0)) && ((x < WINDOW_WIDTH) && (y < WINDOW_HEIGHT)))
	{
		pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
		*(int *)pixel = color;
	}
}

//returns color gradient increment
static int	gradient_increment(int start, int end, int stepc, int stepn)
{
	float	step_r;
	float	step_g;
	float	step_b;

	step_r = (float)(((end >> 16) & 0xFF) -
				((start >> 16) & 0xFF)) /(float)stepc;
	step_g = (float)(((end >> 8) & 0xFF) -
				((start >> 8) & 0xFF)) / (float)stepc;
	step_b = (float)((end & 0xFF) - (start & 0xFF)) / (float)stepc;
	return  (((int)(((start >> 16) & 0xFF) + stepn * step_r) << 16) +
			((int)(((start >> 8) & 0xFF) + stepn * step_g) << 8) +
			((int)(((start >> 0) & 0xFF) + stepn * step_b) << 0));
}

//draws sky and floor with slight gradient
static void	draw_background(t_data *data)
{
	int	x;
	int	y;

	y = 0;
	while (y < WINDOW_HEIGHT)
	{
		x = 0;
		while (x < WINDOW_WIDTH)
		{
			if (y < WINDOW_HEIGHT/2)
				img_pixel_put(&data->img, x, y,
					gradient_increment(data->scene.colors[1][0],
					gradient_increment(data->scene.colors[1][0], 0, 4, 3),
					WINDOW_HEIGHT/2, y));
			else
				img_pixel_put(&data->img, x, y,
					gradient_increment(gradient_increment(
						data->scene.colors[0][0],0, 4, 3), data->scene.colors[0][0],
						WINDOW_HEIGHT/2, y - WINDOW_HEIGHT/2));
			x++;
		}
		y++;
	}
}

//rendering loop
int	render(t_data *data)
{
	data->img.mlx_img = mlx_new_image(data->ptrs.mlx, WINDOW_WIDTH,
			WINDOW_HEIGHT);
	data->img.addr = mlx_get_data_addr(data->img.mlx_img, &data->img.bpp,
			&data->img.line_len, &data->img.endian);
	draw_background(data);
	//draw foreground
	//draw ui
	mlx_put_image_to_window(data->ptrs.mlx, data->ptrs.win,
		data->img.mlx_img, 0, 0);
	mlx_destroy_image(data->ptrs.mlx, data->img.mlx_img);
	return (0);
}
