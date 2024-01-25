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

//	suggestion for a differently structured parser, using newly suggestend error funtion and calling subsequent functions needed to interpret arguments directly

//	i wrote this while in the night i couldnt sleep, i dont remember what purpose i had in mind for it

//	saves arguments to their respective variables
// int process_arguments(int ac, char **av, t_game *game)
// {
// 	//map parser
// 	//texture parser
// 	//floor and ceiling colour parser
// }

// int		validate_arguments(int ac, char **av, t_game *game)
// {
// 	if (ac != 2)
// 		return(error_return(0, "invalid number of arguments", 3));
// 	if (ft_strncmp(av[1] + ft_strlen(av[1]) - 4, ".cub", 4))
// 		return(error_return(0, "map file must have .cub ending", 4));
// 	game->scene.map.map_name = av[1];
// 	return (process_arguments(ac, av, game));
// }


int		interpret_arguments(int an, char **ac, t_game *game, char **map_fn)
{
	if (an != 2)
		return (set_return_error(game, "Expecting one argument (map file)"));
	if (ft_strncmp(ac[1] + ft_strlen(ac[1]) - 4, ".cub", 4))
		return (set_return_error_extra(game,
			"Expecting .cub file ending for map, found file name ", ac[1]));
	*map_fn = ac[1];
	return (0);
}

typedef struct s_parse_helper
{
	t_game *game;
	t_texture *textures;
	int found_colors[2];
	int found_map_start;
	int found_map_end;
	int line_idx;
	int map_start_idx;
	int map_end_idx;
	int map_max_line_length;
	int interpreted_this_line;
} t_parse_helper;

static void pass1_mark_map_end(t_parse_helper* ph)
{
	ph->found_map_end = 1;
	ph->map_end_idx = ph->line_idx;
}

static int pass1_parse_color(t_parse_helper* ph, char which, char *rest)
{
	char **parts;
	int i;
	long 	colortemp;
	int coloridx;
	int *color;

	if (which == 'F')
		coloridx = 0;
	else if (which == 'C')
		coloridx = 1;
	else
		return (set_return_error(ph->game, "Unexpected color line"));

	ph->found_colors[coloridx] = 1;
	color = &ph->game->scene.colors[coloridx][0];

	parts = ft_split(rest, ',');
	if (!parts)
		return (set_return_error(ph->game, "malloc error"));
	
	i = 0;
	while(parts[i] && i < 3)
	{
		colortemp = ft_atoi(parts[i]);
		if (colortemp <= 0 || colortemp >= 256)
		{
			// TODO free parts of parts and parts
			return (set_return_error_extra(ph->game, "Unexpected color value, found ", parts[i]));
		}
		*color = (int)colortemp;
		++color;
		i++;
	}
	if (i < 3 || (i == 3 && parts[i]))
	{
			// TODO free parts of parts and parts
			return (set_return_error_extra(
				ph->game, "Expect 3 color components, found the following color spec ", rest));
	}
	// TODO free parts of parts and parts
	ph->interpreted_this_line = 1;
	return (0);
}

static int pass1_parse_texture(t_parse_helper* ph, t_texture *texture, char *rest)
{
	char *s;

	s = ft_strtrim(rest, " \t\r\n");
	if (!s)
		return (set_return_error(ph->game, "Unexpected empty texture string rest"));
	// TODO test if texture has .xmp file ending
	texture->filename = s;
	ph->interpreted_this_line = 1;
	return (0);
}

/*
   check if line is only spaces
     if yes and found_map_start then found_map_end = 1, record map end idx
     if yes and not found_map_start then skip
     if not and found_map_end then error
   check if we are not in the map and the line is longer than 2 characters, then parse textures/colors
   else assume it is a map part */
static int pass1_classify_line(char* line_temp, t_parse_helper* ph)
{
	char	*s;
	int		linelength;

	//printf("classifying line %s", line_temp);
	ph->line_idx++;
	s = ft_strtrim(line_temp, " \t\r\n");
	if (!s || s[0] == 0)
	{
		if (s)
			free(s);
		if (ph->found_map_start && !ph->found_map_end)
			pass1_mark_map_end(ph);
	}
	else
	{
		if (ph->found_map_end)
		{
			free(s);
			return (set_return_error(ph->game, "Empty line within map is not allowed"));
		}
		ph->interpreted_this_line = 0;
		if (!ph->found_map_start && s[1] != 0 && s[2] != 0)
		{
			if (s[0] == 'C' || s[0] == 'F')
				if (pass1_parse_color(ph, s[0], s + 1))
					return (1);
			// indexes see scene.c
			if (s[0] == 'N' && s[1] == 'O')
				if (pass1_parse_texture(ph, &ph->textures[0], s + 2))
					return (1);
			if (s[0] == 'E' && s[1] == 'A')
				if (pass1_parse_texture(ph, &ph->textures[1], s + 2))
					return (1);
			if (s[0] == 'S' && s[1] == 'O')
				if (pass1_parse_texture(ph, &ph->textures[2], s + 2))
					return (1);
			if (s[0] == 'W' && s[1] == 'E')
				if (pass1_parse_texture(ph, &ph->textures[3], s + 2))
					return (1);
		}
		free(s);
		if (!ph->found_map_start && !ph->interpreted_this_line)
		{
			// everything we cannot interpret should be a map, if it is not, we will detect in pass 2
			// this creates maybe strange error messages, but it should accept all correct maps and reject all incorrect ones
			ph->found_map_start = 1;
			ph->map_start_idx = ph->line_idx;
		}
		if (ph->found_map_start && !ph->found_map_end)
		{
			linelength = ft_strlen(line_temp);
			if (linelength > ph->map_max_line_length)
				ph->map_max_line_length = linelength;
		}
	}
	return (0);
}

/* set map size and map_start_line */
static int pass1_finalize(t_parse_helper* ph, int* map_start_line)
{
	if (!ph->found_colors[0] || !ph->found_colors[1])
		return (set_return_error(ph->game, "Expect floor and ceiling colors"));
	*map_start_line = ph->map_start_idx;
	ph->game->scene.map.map_height = ph->map_end_idx - ph->map_start_idx;
	ph->game->scene.map.map_width = ph->map_max_line_length;
	//printf("found a map of height %d and width %d, starting in line %d of the map file\n", ph->game->scene->map_height, ph->game->scene->map_width, *map_start_line);
	return (0);
}

/* check if each line is something expected
   record the first line of the map
   record map width and height into the scene
   
   open file
   read with get next line library, count lines
   check if line is only spaces
     if yes and found_map_start then found_map_end = 1, record map height in scene
     if yes and not found_map_start then skip
     if not and found_map_end then error
   check if line is longer than 2 characters, if not error
   check if line starts with texture or floor/ceiling/(future: sprites) code or with sprite code, if not found_map_start = 1, record map_start_line
   if found_map_start set scene map width to max(scene map with, length of current line in file)
   close file
   */
int parse_mapfile_pass_1(char *map_fn, t_game *game, int *map_start_line)
{
	int		map_fd;
	char	*line_temp;
	t_parse_helper ph;

	ft_memset(&ph, 0, sizeof(t_parse_helper));
	ph.game = game;
	ph.textures = game->scene.textures;
	map_fd = open(map_fn, O_RDONLY);
	if (map_fd == -1)
		return (set_return_error(game, "Could not open map file."));

	while (1)
	{
		line_temp = get_next_line(map_fd);
		if (line_temp == NULL)
			break;
		if (pass1_classify_line(line_temp, &ph))
			break;
		free(line_temp);
	}
	close(map_fd);

	if (line_temp == NULL) // if the loop above left because the file end was found
	{
		if (ph.found_map_start && !ph.found_map_end)
			pass1_mark_map_end(&ph);
		pass1_finalize(&ph, map_start_line);
		return (0);
	}
	else
	{
		free(line_temp);
		return (1);
	}
}

/*********************************************************************************/
/** split here into files ********************************************************/
/*********************************************************************************/

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

int pass2_found_player(t_convert_helper* ph2, int x, int y, int dx, int dy)
{
	// printf("found player at x %d y %d facing dx %d dy %d\n", x, y, dx, dy);
	if (ph2->found_player)
		return (set_return_error(ph2->game, "Found more than one player"));

	ph2->scene->player.pos_x = x;
	ph2->scene->player.pos_y = y;
	ph2->scene->player.look_x = dx;
	ph2->scene->player.look_y = dy;
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
		ret = pass2_found_player(ph2, x, y, 0, 1);
	else if (source == 'S')
		ret = pass2_found_player(ph2, x, y, 0, -1);
	else if (source == 'W')
		ret = pass2_found_player(ph2, x, y, -1, 0);
	else if (source == 'E')
		ret = pass2_found_player(ph2, x, y, 1, 0);
	return (ret);
}

/* go over each character, counting columns
   handle the player
   otherwise convert characters from file to map representation */
int pass2_do_map_line(char *line, int y, t_convert_helper* ph2)
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
	}
	if (line_temp)
		free(line_temp);

	// TODO final checks: found player?
	close(map_fd);

	return (0);
}

/*********************************************************************************/
/** split here into files ********************************************************/
/*********************************************************************************/


/* check if player is correctly enclosed in walls in the map
   returns 1 on error else 0 */
int parse_final_checks(t_scene* scn)
{
	(void)scn;
	// flood fill, take from so_long
	// set visited areas to 100+<value> and then reset
	return (0);
}

/*********************************************************************************/
/** split here into files ********************************************************/
/*********************************************************************************/

int	**allocate_map(int rows, int cols)
{
	int	i;
	int **m;
	
	m = (int**)malloc(rows*sizeof(int*));
	if (!m)
		return (NULL);
	i = 0;
	while (i < rows)
	{
		m[i] = (int*)malloc(cols*sizeof(int));
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

int		parse_level(char *map_fn, t_game *game)
{
	t_scene	*scn;
	int	map_start_line;

	scn = &game->scene;

	if (parse_mapfile_pass_1(map_fn, game, &map_start_line))
		return (set_return_error(game, "parsing pass 1 failed"));

	scn->map.map = allocate_map(scn->map.map_height, scn->map.map_width);
	if (!scn->map.map)
		return (set_return_error(game, "allocating map failed")); 

	if (parse_mapfile_pass_2(map_fn, game, map_start_line))
	{
		// deallocate
		return (set_return_error(game, "parsing pass 2 failed"));
	}

	if (parse_final_checks(scn))
	{
		// deallocate
		return (set_return_error(game, "parsing final checks failed"));
	}

	return (0);
}
