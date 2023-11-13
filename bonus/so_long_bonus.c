/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 08:17:55 by amarabin          #+#    #+#             */
/*   Updated: 2023/11/12 19:49:05 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

static t_game	*init_game2(t_game *game, int fd)
{
	game->moves = 0;
	game->exits = 0;
	game->colls = 0;
	game->vills = 0;
	game->is_safe = 0;
	game->run = 0;
	null_a(game->tgt_el_q, TGT_Q_LEN);
	game->hero_clock = c_alloc(game, sizeof(struct timespec));
	game->vill_clock = c_alloc(game, sizeof(struct timespec));
	game->msg_clock = c_alloc(game, sizeof(struct timespec));
	game->v_paths_m = NULL;
	game->c_paths_m = NULL;
	game->c_points = NULL;
	if (read_map(game, fd) == -1 || init_mlx(game) == -1)
		c_throw(game, NULL, NULL);
	close(fd);
	return (game);
}

static t_game	*init_game(char *file)
{
	t_game	*game;
	int		fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		c_throw(NULL, strdp("Unable to open file: "), c_strerror());
	game = (t_game *)malloc(sizeof(t_game));
	if (!game)
		c_throw(NULL, NULL, NULL);
	game->is_on = 1;
	game->msg_out = NULL;
	game->map = NULL;
	game->o_map = NULL;
	game->map_h = 0;
	game->map_w = -1;
	game->hero = 0;
	game->hero_r = 0;
	game->hero_c = 0;
	return (init_game2(game, fd));
}

int	main(int argc, char **argv)
{
	t_game	*game;

	game = NULL;
	if (argc != 2)
		return (err(strdp("Missing map path\n"), NULL));
	game = init_game(argv[1]);
	if (game == NULL)
		return (1);
	free_game(game);
	return (0);
}
