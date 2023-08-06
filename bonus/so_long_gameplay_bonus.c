/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_gameplay_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:24:07 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/03 23:11:47 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

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
		return (out(strdup("Can't exit without all the keys"), NULL, game));
	else if (game->map[r][c] == 'E' || game->map[r][c] == 'V')
	{
		mlx_loop_end(game->mlx);
		if (game->map[r][c] == 'V')
			return (out(strdup("You Lose!"), NULL, game));
		return (out(strdup("You Win!"), NULL, game));
	}
	if (game->map[r][c] == 'C')
	{
		game->colls -= 1;
		game->map[r][c] = 0;
		out(itoa(game->colls), strdup(" keys left"), game);
	}
	game->map[game->hero_r][game->hero_c] = '0';
	p_img(game, game->imgs.floor, game->hero_r, game->hero_c);
	game->hero_r = r;
	game->hero_c = c;
	p_img(game, game->imgs.floor, r, c);
	p_img(game, game->imgs.hero, r, c);
	game->moves += 1;
	return (out(itoa(game->moves), strdup(" moves"), game));
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
		return (out(strdup("Quitting"), NULL), game);
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

/**
 * @brief Renders the next frame of the game.
 *
 * This function updates the game state for the next frame.
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
	int		vill_c;
	t_point	*p;
	t_point	*h;
	t_point	*v;

	if (elapsed_time(game->start) > 1000)
	{
		clock_gettime(CLOCK_MONOTONIC, game->start);
		vill_c = 0;
		while (vill_c < game->vills)
		{
			v = game->a_vills[vill_c];
			h = create_point(game->hero_r, game->hero_c, 0);
			p = find_shortest_path(v, h, game);
			if (p == NULL)
				return ;
			if (p->r == game->hero_r && p->c == game->hero_c)
			{
				mlx_loop_end(game->mlx);
				return (out(strdup("You Lose!"), NULL, game));
			}
			p_img(game, game->imgs.floor, v->r, v->c);
			game->map[v->r][v->c] = '0';
			if (v->val)
			{
				p_img(game, game->imgs.coll, v->r, v->c);
				game->map[v->r][v->c] = 'C';
			}
			free(v);
			free(h);
			game->a_vills[vill_c] = p;
			p->val = 0;
			if (game->map[p->r][p->c] == 'C')
				p->val = 1;
			game->map[p->r][p->c] = 'V';
			p_img(game, game->imgs.floor, p->r, p->c);
			p_img(game, game->imgs.vill, p->r, p->c);
			vill_c++;
		}
	}
}
