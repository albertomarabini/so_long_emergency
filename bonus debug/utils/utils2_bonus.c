/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 17:31:51 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/22 00:32:19 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

static void	fill_map_unsafe_spots(t_game *gm, char **map, t_point p, int ray)
{
	if (strchr("1E", map[p.r][p.c]) || (p.r == gm->hero_r && p.c == gm->hero_c)
		|| ray == 0)
		return ;
	map[p.r][p.c] = '1';
	fill_map_unsafe_spots(gm, map, create_p(p.r - 1, p.c, 0, 0), ray - 1);
	fill_map_unsafe_spots(gm, map, create_p(p.r + 1, p.c, 0, 0), ray - 1);
	fill_map_unsafe_spots(gm, map, create_p(p.r, p.c - 1, 0, 0), ray - 1);
	fill_map_unsafe_spots(gm, map, create_p(p.r, p.c + 1, 0, 0), ray - 1);
}

static void print_map(char **o_map) {
    if (o_map == NULL) {
        printf("The map is NULL.\n");
        return;
    }

    for (int i = 0; o_map[i] != NULL; i++) {
        printf("%s\n", o_map[i]);
    }
}

t_game	*create_safe_game(t_game *gm, int safe_w)
{
	t_game	*safe;
	int		r;
	int		c;

	safe = (t_game *)c_alloc(gm, sizeof(t_game));
	safe->map = duplicate_map(gm);
	r = 0;
	while (r < gm->map_h)
	{
		c = 0;
		while (gm->map_w > c++)
			if (safe->map[r][c - 1] == 'V')
			{
				fill_map_unsafe_spots(gm, safe->map, create_p(r, c - 1, 0, 0),
					safe_w);
			}
		r++;
	}
	print_map(safe->map);
	safe->is_safe = 1;
	safe->ancestor = (void *)gm;
	safe->map_h = gm->map_h;
	safe->map_w = gm->map_w;
	safe->hero_c = gm->hero_c;
	safe->hero_r = gm->hero_r;
	safe->a_corners = gm->a_corners;
	safe->a_vills = gm->a_vills;
	safe->a_colls = gm->a_colls;
	safe->a_exits = gm->a_exits;
	safe->vills = gm->vills;
	safe->colls = gm->colls;
	safe->exits = gm->exits;
	null_a(safe->tgt_el_q, TGT_Q_LEN);
	return (safe);
}

/**
 * creates a new row for paths initialized at INT_MAX
 */
static int	*init_path_row(t_game *game)
{
	int	*row;
	int	i;

	row = (int *)c_alloc(game, sizeof(int) * (game->map_w));
	i = 0;
	while (i < game->map_w)
		row[i++] = INT_MAX;
	return (row);
}

/**
 * initializes a new matrix of paths
 */
int	**init_path_matrix(t_game *game)
{
	int	**paths;
	int	r;

	paths = (int **)c_alloc(game, sizeof(int *) * (game->map_h + 1));
	r = 0;
	while (r < game->map_h)
		paths[r++] = init_path_row(game);
	paths[r] = NULL;
	return (paths);
}
/**
 * object that specifies the behavior of
 * the function sel_tgt
 */
t_sel_tgt_obj_mechanics	sto_mech(t_point **(*sort)(t_point **), int best)
{
	t_sel_tgt_obj_mechanics	m;

	m.sort = sort;
	m.best = best;
	return (m);
}
