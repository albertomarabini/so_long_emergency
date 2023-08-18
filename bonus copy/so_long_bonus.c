/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 08:17:55 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/17 12:22:27 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

void	free_image(void *mlx, void *img)
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

void	free_game(t_game *game)
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
	free(game->msg_out);
	free_sprite(game, game->hero_spt);
	if (game->vill_spt)
		free_sprite_a(game, game->vill_spt);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
	free(game);
}

static t_game	*init_game(char *file)
{
	t_game	*game;
	int		fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		err(strdup("Unable to open file: "), c_strerror());
	game = (t_game *)malloc(sizeof(t_game));
	if (game == NULL)
		exit(1);
	game->is_on = 1;
	game->msg_out = NULL;
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
	null_a(game->tgt_el_q, TGT_Q_LEN);
	game->hero_clock = c_alloc(game, sizeof(struct timespec));
	game->vill_clock = c_alloc(game, sizeof(struct timespec));
	game->msg_clock = c_alloc(game, sizeof(struct timespec));
	if (fd < 0 || read_map(game, fd) == -1 || init_mlx(game) == -1)
	{
		free_game(game);
		exit(1);
	}
	game->v_paths_m = NULL;
	game->c_paths_m = NULL;
	game->c_points = NULL;
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
	free_game(game);
	return (0);
}
