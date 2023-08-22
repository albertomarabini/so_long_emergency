/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 17:31:51 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/22 17:26:14 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

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
t_sel_tgt_obj_mechanics	sel_tgt_mech(t_point **(*sort)(t_point **), int best)
{
	t_sel_tgt_obj_mechanics	m;

	m.sort = sort;
	m.best = best;
	return (m);
}
