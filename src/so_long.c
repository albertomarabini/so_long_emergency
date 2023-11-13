/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 08:17:55 by amarabin          #+#    #+#             */
/*   Updated: 2023/11/12 19:57:29 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	free_image(void *mlx, void *img)
{
	if (img != NULL)
		mlx_destroy_image(mlx, img);
}

static void	free_images(t_game *game)
{
	free_image(game->mlx, game->imgs.w_tl);
	free_image(game->mlx, game->imgs.w_tr);
	free_image(game->mlx, game->imgs.w_bl);
	free_image(game->mlx, game->imgs.w_br);
	free_image(game->mlx, game->imgs.w_l);
	free_image(game->mlx, game->imgs.w_r);
	free_image(game->mlx, game->imgs.w_t);
	free_image(game->mlx, game->imgs.w_b);
	free_image(game->mlx, game->imgs.obst);
	free_image(game->mlx, game->imgs.coll);
	free_image(game->mlx, game->imgs.exit);
	free_image(game->mlx, game->imgs.hero);
	free_image(game->mlx, game->imgs.floor);
}

int	free_game(t_game *game)
{
	int	i;

	i = 0;
	if (game->map != NULL)
	{
		while (i < game->map_h)
		{
			if (game->map[i])
				free(game->map[i]);
			i++;
		}
		free(game->map);
	}
	free_images(game);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	out(strdp("Goodbye!\n"), NULL);
	if (game->win)
		free(game->mlx);
	free(game);
	exit(0);
	return (0);
}

static t_game	*init_game(char *file)
{
	t_game	*game;
	int		fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (err(strdp("Unable to open file\n"), NULL), NULL);
	game = (t_game *)malloc(sizeof(t_game));
	game->map = NULL;
	game->map_h = 0;
	game->map_w = -1;
	game->hero = 0;
	game->hero_r = 0;
	game->hero_c = 0;
	game->moves = 0;
	game->exit = 0;
	game->colls = 0;
	if (fd < 0 || read_map(game, fd) == -1 || init_mlx(game) == -1)
	{
		free_game(game);
		return (NULL);
	}
	close(fd);
	return (game);
}

void	leaks(void)
{
	system("leaks so_long");
}

int	main(int argc, char **argv)
{
	t_game	*game;

	atexit(&leaks);
	game = NULL;
	if (argc != 2)
		return (err(strdp("Missing map path\n"), NULL));
	game = init_game(argv[1]);
	if (game == NULL)
		return (1);
	free_game(game);
	return (0);
}
