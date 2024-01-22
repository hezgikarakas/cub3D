/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 12:47:02 by karakasschu       #+#    #+#             */
/*   Updated: 2024/01/22 06:24:30 by jkatzenb         ###   ########.fr       */
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

//sets window name and inits mlx pointers
void	initialize(t_data *data, char *mapname)
{
	char	*name;

	name = ft_strjoin("CUBE3D - ", mapname);
	data->ptrs.mlx = mlx_init();
	if (data->ptrs.mlx == NULL)
	{
		perror("mlx init failed");
		exit(2);
	}
	data->ptrs.win = mlx_new_window(data->ptrs.mlx, WINDOW_WIDTH,
			WINDOW_HEIGHT, name);
	free(name);
	if (data->ptrs.win == NULL)
	{
		free(data->ptrs.mlx);
		perror("window creation failed");
		exit(3);
	}
}

static void	temp_interpreter_bypass(int ac, char **av, t_data *data)
{
	if (ac || av || !ac)
	{
		data->scene.colors[1][0] = 0x07c2ed;
		data->scene.colors[0][0] = 0x37dd08;
	}
}

int	main(int argc, char** argv)
{
	t_data	*data;
	// t_game		game;
	// char*		map_fn;

	// ft_memset(&game, 0, sizeof(t_game));
	data = (t_data *)malloc(sizeof(t_data));
	if (!data)
	{
		perror("malloc failed");
		return (1);
	}
	// if (interpret_arguments(argc, argv, &game, &map_fn))
	// 	return (print_return_error(&game));
	temp_interpreter_bypass(argc, argv, data);
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
