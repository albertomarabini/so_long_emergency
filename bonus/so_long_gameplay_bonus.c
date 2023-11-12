/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_gameplay_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:24:07 by amarabin          #+#    #+#             */
/*   Updated: 2023/10/29 10:53:48 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

static void	catch_coll(t_game *game, int r, int c)
{
	int	i;

	game->colls -= 1;
	game->map[r][c] = '0';
	game->o_map[r][c] = '0';
	null_a(game->tgt_el_q, TGT_Q_LEN);
	i = 0;
	while (game->a_colls[i])
	{
		if (game->a_colls[i]->r == r && game->a_colls[i]->c == c)
		{
			game->a_colls[i]->val = 1;
			break ;
		}
		i++;
	}
}

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
		return (out(strdup("Invalid move"), NULL, game));
	else if (game->map[r][c] == 'E' && game->colls != 0)
		return (out(strdup("Can't exit without all the keys\n"), NULL, game));
	else if (game->map[r][c] == 'E' || game->map[r][c] == 'V')
	{
		game->is_on = 0;
		if (game->map[r][c] == 'V')
			return (out(strdup("You Lose!"), NULL, game));
		return (out(strdup("You Win!"), NULL, game));
	}
	if (game->map[r][c] == 'C')
		catch_coll(game, r, c);
	game->map[game->hero_r][game->hero_c] = '0';
	p_img(game, game->i.floor, game->hero_r, game->hero_c);
	game->hero_r = r;
	game->hero_c = c;
	p_img(game, game->i.floor, r, c);
	p_img(game, get_sprite_frame(game->hero_spt), r, c);
	game->moves += 1;
	return (out(NULL, NULL, game));
}

/**
 * Handles keypress events for the game.If a movement key (W, A, S, D or
 * arrows) is pressed, attempts the move, if ESC quits the game
 */
int	on_keypress(int key, t_game *game)
{
	if (!game->is_on)
		return (0);
	if (key == 53)
	{
		free_game(game);
		return (out(strdup("Quitting\n"), NULL, game));
	}
	else if (key == 123 || key == 0)
		move_to(game->hero_r, game->hero_c - 1, game);
	else if (key == 126 || key == 13)
		move_to(game->hero_r - 1, game->hero_c, game);
	else if (key == 124 || key == 2)
		move_to(game->hero_r, game->hero_c + 1, game);
	else if (key == 125 || key == 1)
		move_to(game->hero_r + 1, game->hero_c, game);
	return (0);
}

/**
 * @brief Renders the next frame of the game.
 *
 * This function updates the game state for the next frame.
 * After frame interval has passed it resets the clock;
 *
 * For each villain it finds the shortest path to the hero and updates the
 * villain's position. If a villain reaches the hero's position, it ends
 * the game loop and declares the game lost.
 *
 * @param game Pointer to a t_game struct that represents the game state.
 * @return int Returns 0 if the game continues, or non-zero if the game ends.
 *
 * @remarks Attention to the frees: v is a t_point * and is freed and
 * by a new one, 'p', each time. The first has to be freed, the second will
 * stay in the a_vills array.
 */
int	render_next_frame(t_game *game)
{
	t_point	p;

	if (!game->is_on && elapsed_time(game->hero_clock, game) > GO_TICK)
		free_game(game);
	else if (game->is_on)
	{
		if (game->msg_out && elapsed_time(game->msg_clock, game) > MSG_TICK)
		{
			free(game->msg_out);
			game->msg_out = NULL;
		}
		if (elapsed_time(game->hero_clock, game) > HERO_TICK)
		{
			clock_gettime(CLOCK_MONOTONIC, game->hero_clock);
			p = find_hero_move(game);
			if (!is_null_p(p))
				move_to(p.r, p.c, game);
		}
		if (elapsed_time(game->vill_clock, game) > VILL_TICK)
		{
			clock_gettime(CLOCK_MONOTONIC, game->vill_clock);
			return (move_vills(game));
		}
	}
	return (0);
}
