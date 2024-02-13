/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pass1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:53:54 by karakasschu       #+#    #+#             */
/*   Updated: 2024/02/13 17:38:47 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

static void	pass1_mark_map_end(t_parse_helper *ph)
{
	ph->found_map_end = 1;
	ph->map_end_idx = ph->line_idx;
}

static void	handle_potential_map_end(t_parse_helper *ph)
{
	if (ph->found_map_start && !ph->found_map_end)
		pass1_mark_map_end(ph);
}

/* check if line is only spaces
     if yes and found_map_start then found_map_end = 1, record map end idx
     if yes and not found_map_start then skip
     if not and found_map_end then error
   check if we are not in the map and the line is longer than 2 characters,
   then parse textures/colors
   else assume it is a map part */
static int	pass1_classify_line(char *line_temp, t_parse_helper *ph)
{
	char	*s;

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
			return (error_return(0, "Empty line in map is not allowed!", -1));
		}
		ph->interpreted_this_line = 0;
		if (pass1_classify_trimmed_line(ph, line_temp, s))
			return (1);
	}
	ph->line_idx++;
	return (0);
}

/* set map size and map_start_line */
static int	pass1_finalize(t_parse_helper *ph, int *map_start_line)
{
	handle_potential_map_end(ph);
	// if (!ph->found_floor_color || !ph->found_ceiling_color)
	// 	return (error_return(0,
	// 			"Did not find both floor and ceiling colors!", -1));
	*map_start_line = ph->map_start_idx;
	ph->game->scene.map.map_height = ph->map_end_idx - ph->map_start_idx;
	ph->game->scene.map.map_width = ph->map_max_line_length;
	return (0);
}

/* check if each line is something expected
   record the first line of the map
   record map width and height into the scene */
int	parse_mapfile_pass_1(char *map_fn, t_game *game, int *map_start_line)
{
	t_parse_helper	ph;

	ft_memset(&ph, 0, sizeof(t_parse_helper));
	ph.game = game;
	ph.map_fd = open(map_fn, O_RDONLY);
	if (ph.map_fd == -1)
		return (error_return_s(1, "Could not open map file ", -1, map_fn));
	while (1)
	{
		ph.line_temp = get_next_line(ph.map_fd);
		if (ph.line_temp == NULL || pass1_classify_line(ph.line_temp, &ph))
			break ;
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
