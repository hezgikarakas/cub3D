/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pass2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:53:54 by karakasschu       #+#    #+#             */
/*   Updated: 2024/02/21 12:19:43 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

/* convert characters from file to map representation
   convert ascii to integer map content
   player position is floor
   space is 0
   map representation = integer of the character (e.g., '4' becomes 4)
   return 0 on success, 1 on error */
static int	pass2_convert_one_field(char source, int *destination)
{
	if (source == ' ')
		*destination = 0;
	else if (source >= '0' && source <= '1')
		*destination = source - '0';
	else if (ft_strchr("NSWE", source))
		*destination = 0;
	else
		return (error_return(0, "Unexpected map char, need [01NSWE ]", -1));
	return (0);
}

/* if player already found, this is an error
   set player position: walls are exactly on given x/y location,
   so we move player by half a grid cell or we might start in a wall
   convert direction into look vector
   compute plane from look direction
   - plane is vector (lookx, looky) rotated leftwards by 90 degrees
     scaled down by 0.66
   - see https://en.wikipedia.org/wiki/Rotation_matrix */
static int	pass2_found_player(t_parse_pass2 *ph2, t_xy pos, float fx, float fy)
{
	if (ph2->found_player)
		return (error_return(0, "Found more than one player", -1));
	ph2->game->player.pos_x = pos.x + 0.5;
	ph2->game->player.pos_y = pos.y + 0.5;
	ph2->game->player.look_x = fx;
	ph2->game->player.look_y = fy;
	ph2->game->player.plane_x = 0.66 * (
			fx * cos(0.5 * 3.14) - fy * sin(0.5 * 3.14));
	ph2->game->player.plane_y = 0.66 * (
			fx * sin(0.5 * 3.14) + fy * cos(0.5 * 3.14));
	ph2->found_player = 1;
	return (0);
}

/* if encountering player character (NSWE) handle that we found a player */
static int	pass2_handle_player(char source, int x, int y, t_parse_pass2 *ph2)
{
	int		ret;
	t_xy	pos;

	ret = 0;
	pos.x = x;
	pos.y = y;
	if (source == 'N')
		ret = pass2_found_player(ph2, pos, 0, -1.0);
	else if (source == 'S')
		ret = pass2_found_player(ph2, pos, 0, 1.0);
	else if (source == 'W')
		ret = pass2_found_player(ph2, pos, -1.0, 0);
	else if (source == 'E')
		ret = pass2_found_player(ph2, pos, 1.0, 0);
	return (ret);
}

/* initialize map row as empty space (0)
   go over each character, counting columns
   handle the player
   otherwise convert characters from file to map representation */
static int	pass2_do_map_line(char *line, int y, t_parse_pass2 *ph2)
{
	int		x;
	t_map	*map;

	map = &ph2->game->scene.map;
	ft_memset(map->map[y], 0, sizeof(int) * map->map_width);
	x = 0;
	while (line[x] && line[x] != '\n' && x < map->map_width)
	{
		if (pass2_handle_player(line[x], x, y, ph2))
			return (1);
		if (pass2_convert_one_field(line[x], &map->map[y][x]))
			return (1);
		x++;
	}
	return (0);
}

/* parse map content into allocated map in scene
   initialize player in scene
   
   open file
   read with get next line library, count lines
   only consider lines from map_start_idx until map_start_idx + map_height - 1
   do not trim line
 
   if player not found, deallocate and error
   if colors not found, deallocate and error
   if textures not found, deallocate and error
   close file */
int	parse_mapfile_pass_2(char *map_fn, t_game *game, int start_idx)
{
	t_parse_pass2	ph2;

	ft_memset(&ph2, 0, sizeof(t_parse_pass2));
	ph2.map_fd = open(map_fn, O_RDONLY);
	if (ph2.map_fd == -1)
		return (error_return_s(1, "Could not open map file: ", -1, map_fn));
	ph2.game = game;
	ph2.line = get_next_line(ph2.map_fd);
	while (ph2.line)
	{
		if (!ph2.error && ph2.line_idx >= start_idx && ph2.line_idx < (
				start_idx + game->scene.map.map_height
			) && pass2_do_map_line(ph2.line, ph2.line_idx - start_idx, &ph2))
			ph2.error = 1;
		ph2.line_idx++;
		free(ph2.line);
		ph2.line = get_next_line(ph2.map_fd);
	}
	if (ph2.line)
		free(ph2.line);
	close(ph2.map_fd);
	if (!ph2.found_player && !ph2.error)
		return (error_return(0, "Found no player!", -1));
	return (ph2.error);
}
