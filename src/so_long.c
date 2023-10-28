/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 08:17:55 by amarabin          #+#    #+#             */
/*   Updated: 2023/10/28 11:18:39 by amarabin         ###   ########.fr       */
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

static void	free_game(t_game *game)
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
	// if (game->mlx)
	// {
	// 	mlx_destroy_display(game->mlx);
		free(game->mlx);
	//}
	free(game);
}

static t_game	*init_game(char *file)
{
	t_game	*game;
	int		fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		err(strdup("Unable to open file\n"), NULL);
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
	return (out(strdup("Goodbye!\n"), NULL));
}
