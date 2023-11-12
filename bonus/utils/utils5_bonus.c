/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 17:41:38 by amarabin          #+#    #+#             */
/*   Updated: 2023/10/29 10:08:13 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

void	free_image(void *mlx, void *img)
{
	if (img != NULL)
		mlx_destroy_image(mlx, img);
}

void	free_map(char **map, int h)
{
	int	i;

	i = 0;
	while (i < h)
	{
		if (map[i])
			free(map[i]);
		i++;
	}
	free(map);
}

static void	free_images(t_game *game)
{
	free_image(game->mlx, game->i.w_tl);
	free_image(game->mlx, game->i.w_tr);
	free_image(game->mlx, game->i.w_bl);
	free_image(game->mlx, game->i.w_br);
	free_image(game->mlx, game->i.w_l);
	free_image(game->mlx, game->i.w_r);
	free_image(game->mlx, game->i.w_t);
	free_image(game->mlx, game->i.w_b);
	free_image(game->mlx, game->i.obst);
	free_image(game->mlx, game->i.coll);
	free_image(game->mlx, game->i.exit);
	free_image(game->mlx, game->i.hero);
	free_image(game->mlx, game->i.floor);
}

static int	free_game2(t_game *game)
{
	free(game->msg_out);
	free_sprite(game, game->hero_spt);
	if (game->vill_spt)
		free_sprite_a(game, game->vill_spt);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->win)
		free(game->mlx);
	free(game);
	exit(0);
	return (0);
}

int	free_game(t_game *game)
{
	if (game->map != NULL)
		free_map(game->map, game->map_h);
	if (game->o_map != NULL)
		free_map(game->o_map, game->map_h);
	free_images(game);
	free_ap(game->a_corners);
	free_ap(game->a_vills);
	free_ap(game->a_colls);
	free_ap(game->a_exits);
	if (game->hero_clock)
		free(game->hero_clock);
	if (game->vill_clock)
		free(game->vill_clock);
	if (game->msg_clock)
		free(game->msg_clock);
	return(free_game2(game));
}
