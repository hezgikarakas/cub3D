/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:53:54 by karakasschu       #+#    #+#             */
/*   Updated: 2024/01/24 15:00:06 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

/* if we visit outside the map, return 1 (error)
 * if we come to a 0, we set to -100 and visit all fields around
 * return 0 */
static int	ft_flood_fill(t_map *map, int x, int y)
{
	if (x < 0 || y < 0 || x >= map->map_width || y >= map->map_height)
		return (error_return(0, "player is not enclosed by walls", -1));
	if (map->map[y][x] == 0)
	{
		map->map[y][x] = -100;
		if (ft_flood_fill(map, x - 1, y)
			|| ft_flood_fill(map, x + 1, y)
			|| ft_flood_fill(map, x, y - 1)
			|| ft_flood_fill(map, x, y + 1))
			return (1);
	}
	return (0);
}

static void	ft_flood_restore(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < game->scene.map.map_height)
	{
		x = 0;
		while (x < game->scene.map.map_width)
		{
			if (game->scene.map.map[y][x] == -100)
				game->scene.map.map[y][x] = 0;
			x++;
		}
		y++;
	}
}

/* check if player is correctly enclosed in walls in the map
   returns 1 on error else 0 */
int	map_final_checks(t_game *game)
{
	int	ret;

	ret = ft_flood_fill(
			&game->scene.map,
			game->player.pos_x, game->player.pos_y);
	ft_flood_restore(game);
	return (ret);
}
