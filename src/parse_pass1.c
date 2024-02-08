/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pass1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:53:54 by karakasschu       #+#    #+#             */
/*   Updated: 2024/01/24 15:00:06 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

typedef struct s_parse_helper
{
	int			map_fd;
	char		*line_temp;
	t_game		*game;
	int			found_floor_color;
	int			found_ceiling_color;
	int			found_map_start;
	int			found_map_end;
	int			line_idx;
	int			map_start_idx;
	int			map_end_idx;
	int			map_max_line_length;
	int			interpreted_this_line;
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
	int *found_color;
	int *color;

	if (which == 'F')
	{
		color = &ph->game->scene.floor_colour;
		found_color = &ph->found_floor_color;
	}
	else if (which == 'C')
	{
		color = &ph->game->scene.ceiling_colour;
		found_color = &ph->found_ceiling_color;
	}
	else
		return (set_return_error(ph->game, "Unexpected color line"));

	*found_color = 1;

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

static void	handle_potential_map_end(t_parse_helper* ph)
{
	if (ph->found_map_start && !ph->found_map_end)
		pass1_mark_map_end(ph);
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
	s = ft_strtrim(line_temp, " \t\r\n");
	if (!s || s[0] == 0)
	{
		if (s)
			free(s);
		handle_potential_map_end(ph);
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
				if (pass1_parse_texture(ph, &ph->game->scene.textures[0], s + 2))
					return (1);
			if (s[0] == 'E' && s[1] == 'A')
				if (pass1_parse_texture(ph, &ph->game->scene.textures[1], s + 2))
					return (1);
			if (s[0] == 'S' && s[1] == 'O')
				if (pass1_parse_texture(ph, &ph->game->scene.textures[2], s + 2))
					return (1);
			if (s[0] == 'W' && s[1] == 'E')
				if (pass1_parse_texture(ph, &ph->game->scene.textures[3], s + 2))
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
			if (line_temp[linelength - 1] == '\n')
				linelength--;
			if (linelength > ph->map_max_line_length)
				ph->map_max_line_length = linelength;
		}
	}
	ph->line_idx++;
	return (0);
}

/* set map size and map_start_line */
static int pass1_finalize(t_parse_helper* ph, int* map_start_line)
{
	handle_potential_map_end(ph);
	if (!ph->found_floor_color || !ph->found_ceiling_color)
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
	t_parse_helper	ph;

	ft_memset(&ph, 0, sizeof(t_parse_helper));
	ph.game = game;
	ph.map_fd = open(map_fn, O_RDONLY);
	if (ph.map_fd == -1)
		return (set_return_error(game, "Could not open map file."));
	while (1)
	{
		ph.line_temp = get_next_line(ph.map_fd);
		if (ph.line_temp == NULL || pass1_classify_line(ph.line_temp, &ph))
			break;
		free(ph.line_temp);
	}
	close(ph.map_fd);
	if (ph.line_temp == NULL)
	{
		pass1_finalize(&ph, map_start_line);
		return (0);
	}
	else
	{
		free(ph.line_temp);
		return (1);
	}
}