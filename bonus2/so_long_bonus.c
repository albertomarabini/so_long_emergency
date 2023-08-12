/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 08:17:55 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/12 05:16:55 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

static void	free_image(void *mlx, void *img)
{
	if (img != NULL)
		mlx_destroy_image(mlx, img);
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

void	free_map(char **map, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (map[i])
			c_frr("", map[i]);
		i++;
	}
	c_frr("", map);
}
static void	free_game(t_game *game)
{
	if (game->map != NULL)
	{
		free_map(game->map, game->map_h);
	}
	if (game->o_map != NULL)
	{
		free_map(game->o_map, game->map_h);
	}
	free_images(game);
	free_p(game->a_corners);
	free_p(game->a_vills);
	free_p(game->a_colls);
	free_p(game->a_exits);
	if (game->hero_clock)
		c_frr("", game->hero_clock);
	if (game->vill_clock)
		c_frr("", game->vill_clock);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		c_frr("", game->mlx);
	}
	c_frr("", game);
}

static t_game	*init_game(char *file)
{
	t_game	*game;
	int		fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		err(strdup("Unable to open file: "), c_strerror());
	game = (t_game *)c_alloc(sizeof(t_game));
	game->map = NULL;
	game->o_map = NULL;
	game->map_h = 0;
	game->map_w = -1;
	game->hero = 0;
	game->hero_r = 0;
	game->hero_c = 0;
	game->moves = 0;
	game->exits = 0;
	game->colls = 0;
	game->vills = 0;
	game->run = 0;
	game->run_unsafe = 0;
	null_a(game->tgt_el, 3);
	//null_a(game->tgt_cr, 3);
	game->hero_clock = c_alloc(sizeof(struct timespec));
	game->vill_clock = c_alloc(sizeof(struct timespec));
	if (fd < 0 || read_map(game, fd) == -1 || init_mlx(game) == -1)
	{
		free_game(game);
		return (NULL);
	}
	close(fd);
	return (game);
}

int	main(int argc, char **argv)
{
	t_game	*game;

	game = NULL;
	if (argc != 2)
		return (err(strdup("Missing map path\n"), NULL));
	game = init_game(argv[1]);
	if (game == NULL)
		return (1);
	out(strdup("Goodbye!"), NULL, game);
	free_game(game);
}
