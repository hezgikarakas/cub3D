/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:38:53 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/01/19 17:39:17 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

static void	img_pixel_put(t_img *img, int x, int y, int color)
{
	char	*pixel;

	if (((x > 0) && (y > 0)) && ((x < WINDOW_WIDTH) && (y < WINDOW_HEIGHT)))
	{
		pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
		*(int *)pixel = color;
	}
}

//test
static void	test_cube(t_data *data)
{
	for (int x = WINDOW_WIDTH/2-20; x <= WINDOW_WIDTH/2+20; x++){
		for (int y = 20; y <= 60; y++){
			img_pixel_put(&data->img, x, y, 0xBEBEEE);
		}
	}
}

int	render(t_data *data)
{
	data->img.mlx_img = mlx_new_image(data->ptrs.mlx, WINDOW_WIDTH,
			WINDOW_HEIGHT);
	data->img.addr = mlx_get_data_addr(data->img.mlx_img, &data->img.bpp,
			&data->img.line_len, &data->img.endian);
	//test
	for (int i = 30; i <= WINDOW_WIDTH - 30; i++){
		img_pixel_put(&data->img, i, WINDOW_HEIGHT / 2, 0xBEBEEE);
	}
	test_cube(data);
	//test end
	mlx_put_image_to_window(data->ptrs.mlx, data->ptrs.win,
		data->img.mlx_img, 0, 0);
	mlx_destroy_image(data->ptrs.mlx, data->img.mlx_img);
	//test
	mlx_string_put(data->ptrs.mlx, data->ptrs.win,
		WINDOW_WIDTH/2-10, 45, 0x000000, "TEST");
	mlx_string_put(data->ptrs.mlx, data->ptrs.win,
		WINDOW_WIDTH/2-10, WINDOW_HEIGHT/2-5, 0xff0000, "TEST");
	//test end
	return (0);
}
