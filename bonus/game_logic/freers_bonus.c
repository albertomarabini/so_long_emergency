/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freers_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 02:24:58 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/17 03:34:27 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

void	free_path(int **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}

void	free_paths_matrix(int ***paths)
{
	int	i;

	i = 0;
	while (paths[i])
		free_path(paths[i++]);
	free(paths);
}

void	free_safe_map(t_game *game)
{
	if (game->map != NULL)
		free_map(game->map, game->map_h);
	free(game);
}

void	free_tgt_path_structs(int ***c_paths_m, t_point **c_points)
{
	free_paths_matrix(c_paths_m);
	free_ap(c_points);
}
