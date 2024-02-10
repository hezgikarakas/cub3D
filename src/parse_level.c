/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_level.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:53:54 by karakasschu       #+#    #+#             */
/*   Updated: 2024/01/24 15:00:06 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

static int	validate_arguments(int ac, char **av, t_game *game)
{
	if (ac != 2)
		return (set_return_error(game, "invalid number of arguments"));
	if (ft_strncmp(av[1] + ft_strlen(av[1]) - 4, ".cub", 4))
		return (set_return_error_extra(game,
				"map file must have .cub ending, found ", av[1]));
	return (0);
}

int	process_arguments(int ac, char **av, t_game *game)
{
	if (validate_arguments(ac, av, game))
		return (1);
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
	if (map_final_checks(game))
	{
		// TODO deallocate?
		return (error_return(0, "map checks failed", -1));
	}
	return (0);
}
