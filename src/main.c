/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 12:47:02 by karakasschu       #+#    #+#             */
/*   Updated: 2024/01/19 16:03:36 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

// called when closing window
static int	close_window(t_data *data)
{
	mlx_loop_end(data->ptrs.mlx);
	return (0);
}

// handles keypresses
static int	handle_keypress(int keysym, t_data *data)
{
	if (keysym == 0xff1b)
		close_window(data);
	return (0);
}

void	initialize(t_data *data, char *mapname)
{
	char	*name;

	name = ft_strjoin("CUBE3D - ", mapname);
	data->ptrs.mlx = mlx_init();
	if (data->ptrs.mlx == NULL)
	{
		perror("mlx init failed");
		exit(1);
	}
	data->ptrs.win = mlx_new_window(data->ptrs.mlx, WINDOW_WIDTH,
			WINDOW_HEIGHT, name);
	free(name);
	if (data->ptrs.win == NULL)
	{
		free(data->ptrs.mlx);
		perror("window creation failed");
		exit(2);
	}
}

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	// char*		map_fn;
	// t_game		game;
	t_data	*data;

	// ft_memset(&game, 0, sizeof(t_game));
	// if (interpret_arguments(an, ac, &game, &map_fn))
	// 	return (print_return_error(&game));
	
	data = (t_data *)malloc(sizeof(t_data));
	if (!data)
		perror("malloc failed");
	initialize(data, "test");
	mlx_loop_hook(data->ptrs.mlx, &render, data);
	mlx_hook(data->ptrs.win, 2, 1L << 0, &handle_keypress, data);
	mlx_hook(data->ptrs.win, 17, 0L, &close_window, data);
	mlx_loop(data->ptrs.mlx);
	mlx_destroy_window(data->ptrs.mlx, data->ptrs.win);
	mlx_destroy_display(data->ptrs.mlx);
	free(data->ptrs.mlx);
	free(data);
	return (0);
}