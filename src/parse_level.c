/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_level.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:53:54 by karakasschu       #+#    #+#             */
/*   Updated: 2024/02/08 16:57:15 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

static void	init_arguments(t_game *game)
{
	// game->scene.map.map_name = 0;
	game->scene.textures[0].filename = 0;
	game->scene.textures[1].filename = 0;
	game->scene.textures[2].filename = 0;
	game->scene.textures[3].filename = 0;
	game->scene.ceiling_str = 0;
	game->scene.floor_str = 0;
}

static int	validate_arguments(int ac, char **av, t_game *game)
{
	int	i;

	if (ac < 2)
		return (set_return_error(game, "invalid number of arguments"));
	if (ft_strncmp(av[1] + ft_strlen(av[1]) - 4, ".cub", 4))
		return (set_return_error_extra(game,
				"map file must have .cub ending, found ", av[1]));
	i = 2;
	while (av[i])
	{
		if (ft_strncmp(av[i], "NO", sizeof(av[i])) == 0)
			game->scene.textures[0].filename = av[i + 1];
		if (ft_strncmp(av[i], "SO", sizeof(av[i])) == 0)
			game->scene.textures[2].filename = av[i + 1];
		if (ft_strncmp(av[i], "WE", sizeof(av[i])) == 0)
			game->scene.textures[3].filename = av[i + 1];
		if (ft_strncmp(av[i], "EA", sizeof(av[i])) == 0)
			game->scene.textures[1].filename = av[i + 1];
		if (ft_strncmp(av[i], "F", sizeof(av[i])) == 0)
			game->scene.floor_str = av[i + 1];
		if (ft_strncmp(av[i], "C", sizeof(av[i])) == 0)
			game->scene.ceiling_str = av[i + 1];
		i++;
	}
	return (0);
}

void	get_textures(t_game *game)
{
	load_texture(game, 0);
	load_texture(game, 1);
	load_texture(game, 2);
	load_texture(game, 3);
}

int	convert_colour(char *colour_str)
{
	char	**rgb;
	int		r;
	int		g;
	int		b;

	rgb = ft_split(colour_str, ',');
	r = ft_atoi(rgb[0]);
	g = ft_atoi(rgb[1]);
	b = ft_atoi(rgb[2]);
	free (rgb[0]);
	free (rgb[1]);
	free (rgb[2]);
	free (rgb);
	return (r << 16 | g << 8 | b);
}

void	get_colours(t_game *game)
{
	game->scene.fog = DEFAULT_DISTANCE_FADE;
	if (!game->scene.ceiling_str)
	{
		printf("couldn't find ceiling colour, using default\n");
		game->scene.ceiling_colour = DEFAULT_SKY;
		game->scene.ceiling_gradient = DEFAULT_SKY_GRADIENT;
	}
	else
	{
		game->scene.ceiling_colour = convert_colour(game->scene.ceiling_str);
		game->scene.ceiling_gradient = 0x95c1c1;
		game->scene.fog = 0x95c1c1;
	}
	if (!game->scene.floor_str)
	{
		printf("couldn't find floor colour, using default\n");
		game->scene.floor_colour = DEFAULT_FLOOR;
		game->scene.floor_gradient = DEFAULT_FLOOR_GRADIENT;
	}
	else
	{
		game->scene.floor_colour = convert_colour(game->scene.floor_str);
		game->scene.floor_gradient = 0x95c1c1;
		game->scene.fog = 0x95c1c1;
	}
}

int	process_arguments(int ac, char **av, t_game *game)
{
	init_arguments(game);
	if (validate_arguments(ac, av, game))
		return (1);
	get_textures(game);
	get_colours(game);
	game->scene.map.map_name = av[1];
	return (parse_level(game->scene.map.map_name, game));
}

int	**allocate_map(int rows, int cols)
{
	int	i;
	int	**m;

	m = (int **)malloc(rows * sizeof(int *));
	if (!m)
		return (NULL);
	i = 0;
	while (i < rows)
	{
		m[i] = (int *)malloc(cols * sizeof(int));
		if (!m[i])
		{
			while (--i >= 0)
				free(m[i]);
			free(m);
			return (NULL);
		}
		i++;
	}
	return (m);
}

int	parse_level(char *map_fn, t_game *game)
{
	t_scene	*scn;
	int		map_start_line;

	scn = &game->scene;
	if (parse_mapfile_pass_1(map_fn, game, &map_start_line))
		return (set_return_error(game, "parsing pass 1 failed"));
	scn->map.map = allocate_map(scn->map.map_height, scn->map.map_width);
	if (!scn->map.map)
		return (set_return_error(game, "allocating map failed")); 
	if (parse_mapfile_pass_2(map_fn, game, map_start_line))
	{
		// TODO deallocate?
		return (set_return_error(game, "parsing pass 2 failed"));
	}
	if (map_final_checks(scn))
	{
		// TODO deallocate?
		return (set_return_error(game, "parsing final checks failed"));
	}
	return (0);
}
