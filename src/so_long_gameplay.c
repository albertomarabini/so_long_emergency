/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_gameplay.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:24:07 by amarabin          #+#    #+#             */
/*   Updated: 2023/07/30 12:53:58 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/**
 * Moves the hero to a specific location in the game map.
 *
 * Checks the game map at the new location. If invalid (i.e., a wall), the move
 * is not made and an error message is printed. If the new location is an exit
 * and the hero has collected all the keys, exits the loop with mlx_loop_end.
 * If the new location is valid, the hero moves there, collecting the key if
 * present.
 */
static int	move_to(int r, int c, t_game *game)
{
	if (game->map[r][c] == '1')
		return (out(strdup("Invalid move\n"), NULL));
	else if (game->map[r][c] == 'E' && game->colls != 0)
		return (out(strdup("Can't exit without all the keys\n"), NULL));
	else if (game->map[r][c] == 'E' && game->colls == 0)
	{
		mlx_loop_end(game->mlx);
		return (out(strdup("You Win!\n"), NULL));
	}
	else if (game->map[r][c] == '0' || game->map[r][c] == 'C')
	{
		if (game->map[r][c] == 'C')
		{
			game->colls -= 1;
			out(itoa(game->colls), strdup(" keys left"));
		}
		game->map[game->hero_r][game->hero_c] = '0';
		p_img(game, game->imgs.floor, game->hero_r, game->hero_c);
		game->hero_r = r;
		game->hero_c = c;
		p_img(game, game->imgs.floor, r, c);
		p_img(game, game->imgs.hero, r, c);
	}
	game->moves += 1;
	return (out(itoa(game->moves), strdup(" moves")));
}

/**
 * Handles keypress events for the game.If a movement key (W, A, S, D or
 * arrows) is pressed, attempts the move, if ESC quits the game
 */
int	on_keypress(int key, t_game *game)
{
	if (key == 65307)
	{
		mlx_loop_end(game->mlx);
		return (out(strdup("Quitting\n"), NULL));
	}
	else if (key == 97 || key == 65361)
		move_to(game->hero_r, game->hero_c - 1, game);
	else if (key == 119 || key == 65362)
		move_to(game->hero_r - 1, game->hero_c, game);
	else if (key == 100 || key == 65363)
		move_to(game->hero_r, game->hero_c + 1, game);
	else if (key == 115 || key == 65364)
		move_to(game->hero_r + 1, game->hero_c, game);
	return (0);
}
