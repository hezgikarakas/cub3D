/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_level_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:53:54 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/03/11 18:18:05 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	validate_arguments(int ac, char **av)
{
	if (ac != 2)
		return (error_return(0, "invalid number of arguments", -1));
	if (ft_strncmp(av[1] + ft_strlen(av[1]) - 4, ".cub", 4))
		return (error_return_s(0,
				"map file must have .cub ending, found ", -1, av[1]));
	return (0);
}

int	process_arguments(int ac, char **av, t_game *game)
{
	if (validate_arguments(ac, av))
		return (1);
	game->scene.map.map_name = av[1];
	return (parse_level(game->scene.map.map_name, game));
}

static int	**allocate_map(int rows, int cols)
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

/* run pass 1, allocate map, run pass 2, do final checks
 * if something fails, the main will deallocate whatever we allocated */
int	parse_level(char *map_fn, t_game *game)
{
	t_scene	*scn;
	int		map_start_line;

	scn = &game->scene;
	if (parse_mapfile_pass_1(map_fn, game, &map_start_line))
		return (1);
	scn->map.map = allocate_map(scn->map.map_height, scn->map.map_width);
	if (!scn->map.map)
		return (error_return(1, "allocating map failed", -1));
	if (parse_mapfile_pass_2(map_fn, game, map_start_line))
		return (1);
	if (map_final_checks(game))
		return (error_return(0, "map checks failed", -1));
	return (0);
}
