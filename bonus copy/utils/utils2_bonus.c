/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 17:31:51 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/18 02:11:22 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

static void	fill_map_unsafe_spots(char **map, int r, int c, int ray)
{
	if (map[r][c] == '1' || ray == 0)
		return ;
	map[r][c] = '1';
	fill_map_unsafe_spots(map, r - 1, c, ray - 1);
	fill_map_unsafe_spots(map, r + 1, c, ray - 1);
	fill_map_unsafe_spots(map, r, c - 1, ray - 1);
	fill_map_unsafe_spots(map, r, c + 1, ray - 1);
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
				fill_map_unsafe_spots(safe->map, r, c - 1, safe_w);
		r++;
	}
	safe->map_h = gm->map_h;
	safe->map_w = gm->map_w;
	safe->hero_c = gm->hero_c;
	safe->hero_r = gm->hero_r;
	safe->a_corners = gm->a_corners;
	safe->a_vills = gm->a_vills;
	safe->vills = gm->vills;
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

	row = (int *)malloc(sizeof(int) * (game->map_w));
	if (!row)
		return (NULL);
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

	paths = (int **)malloc(sizeof(int *) * (game->map_h + 1));
	if (!paths)
		return (NULL);
	r = 0;
	while (r < game->map_h)
	{
		paths[r++] = init_path_row(game);
		if (!paths[r - 1])
		{
			err(strdup("Unable to allocate memory"), c_strerror());
			free_path(paths);
			return (NULL);
		}
	}
	paths[r] = NULL;
	return (paths);
}
/**
 * object that specifies the behavior of
 * the function sel_tgt
 */
t_sel_tgt_obj_mechanics	sto_mech(t_point **(*sort)(t_point **, int),
		int best)
{
	t_sel_tgt_obj_mechanics	m;

	m.sort = sort;
	m.best = best;
	return (m);
}
