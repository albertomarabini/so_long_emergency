/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_gameplay2_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:24:07 by amarabin          #+#    #+#             */
/*   Updated: 2023/11/12 19:49:05 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

static int	process_vill(t_game *game, t_point *v, int vill_c)
{
	t_point	p;

	p = find_shortest_path(*v, hero_p(game), game, 1);
	if (p.val == INT_MAX)
		return (0);
	if (p.r == game->hero_r && p.c == game->hero_c)
	{
		game->is_on = 0;
		return (out(strdp("You Lose!"), NULL, game));
	}
	p_img(game, game->i.floor, v->r, v->c);
	game->map[v->r][v->c] = '0';
	if (game->o_map[v->r][v->c] == 'C')
	{
		p_img(game, game->i.coll, v->r, v->c);
		game->map[v->r][v->c] = 'C';
	}
	free(game->a_vills[vill_c - 1]);
	game->a_vills[vill_c - 1] = create_pp(p.r, p.c, 0, 0);
	if (!game->a_vills[vill_c - 1])
		c_throw(game, NULL, NULL);
	game->map[p.r][p.c] = 'V';
	p_img(game, get_sprite_frame(game->vill_spt[vill_c - 1]), p.r, p.c);
	return (1);
}

int	move_vills(t_game *game)
{
	int		vill_c;
	t_point	*v;

	vill_c = 0;
	while (game->vills > vill_c++)
	{
		v = game->a_vills[vill_c - 1];
		if (!process_vill(game, v, vill_c))
			return (0);
	}
	return (1);
}
