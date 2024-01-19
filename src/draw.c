/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:38:53 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/01/19 17:11:42 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

static void	img_pixel_put(t_img *img, int x, int y, int color)
{
	char	*pixel;

	// pixel = 0;
	if (((x > 0) && (y > 0)) && ((x < WINDOW_WIDTH) && (y < WINDOW_HEIGHT)))
		pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(int *)pixel = color;
}

int	render(t_data *data)
{
	data->img.mlx_img = mlx_new_image(data->ptrs.mlx, WINDOW_WIDTH,
			WINDOW_HEIGHT);
	data->img.addr = mlx_get_data_addr(data->img.mlx_img, &data->img.bpp,
			&data->img.line_len, &data->img.endian);
	//test
	int x = 0;
	int i = 10;
	for (int i = 10; i <= 20; i++){
		img_pixel_put(&data->img, i, 10, 0xBEBEEE);
	}
    mlx_string_put(data->ptrs.mlx, data->ptrs.win,
            10, 10, 0xBEBEEE, "TEST");
	//test end
	mlx_put_image_to_window(data->ptrs.mlx, data->ptrs.win,
		data->img.mlx_img, 0, 0);
	mlx_destroy_image(data->ptrs.mlx, data->img.mlx_img);
	return (0);
}