/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 17:25:32 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/17 04:19:36 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

void	*c_alloc(t_game *game, size_t size)
{
	void	*ptr;
	char	*err;

	ptr = malloc(size);
	if (!ptr)
	{
		err = c_strerror();
		printf("Error\nMemory allocation failure\n");
		if (err && err[0])
			printf(" reason: %s\n", err);
		if (game && game->mlx)
			mlx_loop_end(game->mlx);
		if (game)
			free_game(game);
		exit(1);
	}
	return (ptr);
}

/**
 * @brief Get the elapsed time in milliseconds
 *
 * @param start
 * @return long long
 */
long long	elapsed_time(struct timespec *s, t_game *game)
{
	long long		elapsed;
	struct timespec	*e;

	e = (struct timespec *)c_alloc(game, sizeof(struct timespec));
	clock_gettime(CLOCK_MONOTONIC, e);
	elapsed = (e->tv_sec - s->tv_sec) * 1000 + (e->tv_nsec - s->tv_nsec) / 1e6;
	free(e);
	return (elapsed);
}

// int inl(int *dest, int src)
// {
// 	*dest = src;
// 	return (src);
// }

// Function to instantiate a t_pair
t_pair	new_ppair(t_point a, t_point b)
{
	t_pair	pair;

	pair.a = a;
	pair.b = b;
	return (pair);
}

char	**duplicate_map(t_game *gm)
{
	int		i;
	char	**new_map;

	new_map = (char **)c_alloc(gm, (gm->map_h + 1) * sizeof(char *));
	new_map[gm->map_h] = NULL;
	i = 0;
	while (gm->map_h > i++)
		new_map[i - 1] = strdup(gm->map[i - 1]);
	return (new_map);
}

int	inst_tgt_path_structs(int n, int ****c_paths_m, t_point ****c_points)
{
	*c_paths_m = (int ***)malloc(sizeof(int **) * (n + 1));
	**c_points = (t_point **)malloc(sizeof(t_point *) * (n + 1));
	if (!**c_points)
	{
		free(*c_paths_m);
		return (0);
	}
	return (1);
}
