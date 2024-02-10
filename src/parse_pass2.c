/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pass2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:53:54 by karakasschu       #+#    #+#             */
/*   Updated: 2024/01/24 15:00:06 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

typedef struct s_convert_helper
{
	t_game *game;
	t_scene *scene;
	int line_idx;
	int found_player;
} t_convert_helper;

/* convert characters from file to map representation

   convert ascii to integer map content
   player position is floor
   space is -1
   otherwise map representation = integer of the visible character (e.g., '4' becomes 4)
   we currently only support map fields 0-9 (non-bonus is only 0 and 1)

   return 0 on success, 1 on error */
static int pass2_convert_one_field(char source, int* destination, t_convert_helper* ph2)
{
	if (source == ' ')
		*destination = 0;
	else if (source >= '0' && source <= '9')
		*destination = source - '0';
	else if (ft_strchr("NSWE", source))
		*destination = 0;
	else
		return (set_return_error(ph2->game, "Unexpected map character, expect [0-9NSWE ]"));

	return (0);
}

/*
 * compute plane from look direction
 * plane is vector (lookx, looky)
 * rotated leftwards by 90 degrees
 * scaled down by 0.66
 * see https://en.wikipedia.org/wiki/Rotation_matrix
 */
static int pass2_found_player(t_convert_helper* ph2, int x, int y, int dx, int dy)
{
	float	fx;
	float	fy;

	// printf("found player at x %d y %d facing dx %d dy %d\n", x, y, dx, dy);
	if (ph2->found_player)
		return (set_return_error(ph2->game, "Found more than one player"));
	fx = dx;
	fy = dy;
	ph2->game->player.pos_x = x + 0.5; // walls are exactly on given x/y location, so move player by half a grid cell
	ph2->game->player.pos_y = y + 0.5; // TODO there might be a better way to fix this (putting no +0.5 here and moving walls) but maybe this way is the best way
	ph2->game->player.look_x = fx;
	ph2->game->player.look_y = fy;
	ph2->game->player.plane_x = 0.66 * (fx * cos(0.5 * 3.14) - fy * sin(0.5 * 3.14));
	ph2->game->player.plane_y = 0.66 * (fx * sin(0.5 * 3.14) + fy * cos(0.5 * 3.14));
	ph2->found_player = 1;

	return (0);
}

/* handle the player

   if player (N, S, W, E)
      if player already found, deallocate and error
      else convert into 0 and initialize player in scene to current X/Y and correct direction
*/
static int pass2_handle_player(char source, int x, int y, t_convert_helper* ph2)
{
	int ret;

	ret = 0;
	if (source == 'N')
		ret = pass2_found_player(ph2, x, y, 0, -1);
	else if (source == 'S')
		ret = pass2_found_player(ph2, x, y, 0, 1);
	else if (source == 'W')
		ret = pass2_found_player(ph2, x, y, -1, 0);
	else if (source == 'E')
		ret = pass2_found_player(ph2, x, y, 1, 0);
	return (ret);
}

/* go over each character, counting columns
   handle the player
   otherwise convert characters from file to map representation */
static int pass2_do_map_line(char *line, int y, t_convert_helper* ph2)
{
	int x;

	// reset line in map to -1 (space = outside of map)
	ft_memset(ph2->scene->map.map[y], 0, sizeof(int) * ph2->scene->map.map_width);
	x = 0;
	while (line[x] && line[x] != '\n' && x < ph2->scene->map.map_width)
	{
		// printf("at x %d y %d converting '%c'\n", x, y, line[x]);
		if (pass2_handle_player(line[x], x, y, ph2))
			return(1);
		if (pass2_convert_one_field(line[x], &ph2->scene->map.map[y][x], ph2))
			return(1);

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
int parse_mapfile_pass_2(char *map_fn, t_game *game, int map_start_idx)
{
	int					map_fd;
	char				*line_temp;
	t_convert_helper	ph2;

	map_fd = open(map_fn, O_RDONLY);
	if (map_fd == -1)
		return (set_return_error(game, "Could not open map file."));

	ft_memset(&ph2, 0, sizeof(t_convert_helper));
	ph2.game = game;
	ph2.scene = &game->scene;

	while (ph2.line_idx < (map_start_idx + ph2.scene->map.map_height))
	{
		line_temp = get_next_line(map_fd);
		if (line_temp == NULL)
			break;
		if (ph2.line_idx >= map_start_idx)
			if (pass2_do_map_line(line_temp, ph2.line_idx - map_start_idx, &ph2))
				break;
		ph2.line_idx++;
		free(line_temp);
		line_temp = NULL;
	}
	if (line_temp)
		free(line_temp);

	// TODO final checks: found player?
	close(map_fd);

	return (0);
}
