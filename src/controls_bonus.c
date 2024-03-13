/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 14:13:13 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/03/13 18:19:09 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

/*	handles keypresses for forward and backward movement	*/
static void	move_player(int keysym, t_game *game)
{
	if (keysym == XK_Up || keysym == XK_w)
	{
		if (!game->scene.map.map[(int)((game->player.pos_y + game->player.look_y
					* game->player.movespeed))][(int)(game->player.pos_x)])
			game->player.pos_y += game->player.look_y * game->player.movespeed;
		if (!game->scene.map.map[(int)(game->player.pos_y)]
				[(int)(game->player.pos_x + game->player.look_x
				* game->player.movespeed)])
			game->player.pos_x += game->player.look_x * game->player.movespeed;
	}
	if (keysym == XK_Down || keysym == XK_s)
	{
		if (!game->scene.map.map[(int)(game->player.pos_y - game->player.look_y
				* game->player.movespeed)][(int)(game->player.pos_x)])
			game->player.pos_y -= game->player.look_y * game->player.movespeed;
		if (!game->scene.map.map[(int)(game->player.pos_y)]
				[(int)(game->player.pos_x - game->player.look_x
				* game->player.movespeed)])
			game->player.pos_x -= game->player.look_x * game->player.movespeed;
	}
}

/*	handles keypresses for left and right strafing	*/
static void	strafe_player(int keysym, t_game *game)
{
	if (keysym == XK_Page_Down || keysym == XK_e)
	{
		if (!game->scene.map.map[(int)((game->player.pos_y + game->player.look_x
					* game->player.movespeed))][(int)(game->player.pos_x)])
			game->player.pos_y += game->player.look_x * game->player.movespeed;
		if (!game->scene.map.map[(int)(game->player.pos_y)]
				[(int)(game->player.pos_x - game->player.look_y
				* game->player.movespeed)])
			game->player.pos_x -= game->player.look_y * game->player.movespeed;
	}
	if (keysym == XK_Page_Up || keysym == XK_q)
	{
		if (!game->scene.map.map[(int)(game->player.pos_y - game->player.look_x
				* game->player.movespeed)][(int)(game->player.pos_x)])
			game->player.pos_y -= game->player.look_x * game->player.movespeed;
		if (!game->scene.map.map[(int)(game->player.pos_y)]
				[(int)(game->player.pos_x + game->player.look_y
				* game->player.movespeed)])
			game->player.pos_x += game->player.look_y * game->player.movespeed;
	}
}

/*	handles keypresses for rotating the player	*/
static void	rotate_player(int keysym, t_player *player)
{
	double	old_dirx;
	double	old_planex;
	double	temp_rotspeed;

	temp_rotspeed = player->rotspeed;
	temp_rotspeed *= -1;
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
}

/* handles keypresses	*/
int	handle_keypress(int keysym, t_game *game)
{
	if (keysym == XK_Escape)
		close_window(game);
	if (keysym == XK_Up || keysym == XK_w
		|| keysym == XK_Down || keysym == XK_s)
		move_player(keysym, game);
	if (keysym == XK_Page_Up || keysym == XK_q
		|| keysym == XK_Page_Down || keysym == XK_e)
		strafe_player(keysym, game);
	if (keysym == XK_Left || keysym == XK_a
		|| keysym == XK_Right || keysym == XK_d)
		rotate_player(keysym, &game->player);
	return (0);
}

/* handles mouse input	*/
int	handle_mouse(int x, int y, t_game *game)
{
	(void)y;
	if (x > (WINDOW_WIDTH / 2))
	{
		rotate_player(XK_Right, &game->player);
		mlx_mouse_move(game->ptrs.mlx, game->ptrs.win,
			WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	}
	if (x < (WINDOW_WIDTH / 2))
	{
		rotate_player(XK_Left, &game->player);
		mlx_mouse_move(game->ptrs.mlx, game->ptrs.win,
			WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	}
	return (0);
}
