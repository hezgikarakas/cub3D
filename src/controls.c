/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 14:13:13 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/01/25 14:15:03 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

//	called when closing window
int	close_window(t_game *game)
{
	mlx_loop_end(game->ptrs.mlx);
	return (0);
}

//	handles keypresses for forward and backward movement
static int	move_player(int keysym, t_game *game)
{
	if (keysym == XK_Up || keysym == XK_w)
	{
		if (!game->scene.map.map[(int)(game->player.pos_x + game->player.look_x
				* game->player.movespeed)][(int)(game->player.pos_y)])
			game->player.pos_x += game->player.look_x * game->player.movespeed;
		if (!game->scene.map.map[(int)(game->player.pos_x)]
				[(int)(game->player.pos_y + game->player.look_y
				* game->player.movespeed)])
			game->player.pos_y += game->player.look_y * game->player.movespeed;
	}
	if (keysym == XK_Down || keysym == XK_s)
	{
		if (!game->scene.map.map[(int)(game->player.pos_x - game->player.look_x
				* game->player.movespeed)][(int)(game->player.pos_y)])
			game->player.pos_x -= game->player.look_x * game->player.movespeed;
		if (!game->scene.map.map[(int)(game->player.pos_x)]
				[(int)(game->player.pos_y - game->player.look_y
				* game->player.movespeed)])
			game->player.pos_y -= game->player.look_y * game->player.movespeed;
	}
	return (0);
}

//	handles keypresses for rotating the player
static int	rotate_player(int keysym, t_player *player)
{
	double	old_dirx;
	double	old_planex;
	double	temp_rotspeed;

	temp_rotspeed = player->rotspeed;
	if (keysym == XK_Right || keysym == XK_d)
		temp_rotspeed *= -1;
	old_dirx = player->look_x;
	player->look_x = player->look_x * cos(temp_rotspeed)
		- player->look_y * sin(temp_rotspeed);
	player->look_y = old_dirx * sin(temp_rotspeed)
		+ player->look_y * cos(temp_rotspeed);
	old_planex = player->plane_x;
	player->plane_x = player->plane_x * cos(temp_rotspeed)
		- player->plane_y * sin(temp_rotspeed);
	player->plane_y = old_planex * sin(temp_rotspeed)
		+ player->plane_y * cos(temp_rotspeed);
	return (0);
}

// handles keypresses
int	handle_keypress(int keysym, t_game *game)
{
	if (keysym == XK_Escape)
		close_window(game);
	if (keysym == XK_Up || keysym == XK_w
		|| keysym == XK_Down || keysym == XK_s)
		move_player(keysym, game);
	if (keysym == XK_Left || keysym == XK_a
		|| keysym == XK_Right || keysym == XK_d)
		rotate_player(keysym, &game->player);
	return (0);
}
