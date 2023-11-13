/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:41:20 by amarabin          #+#    #+#             */
/*   Updated: 2023/11/12 22:13:38 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

static void	print_score(t_game *game, int i_out, char *msg, int y_offset)
{
	char	*out;
	int		h;
	int		col;

	col = 0x00FFFFFF;
	h = game->map_h * ASSET_SIZE;
	out = itoa(i_out);
	if (!out)
		c_throw(game, NULL, NULL);
	mlx_string_put(game->mlx, game->win, 10, h + y_offset, col, msg);
	mlx_string_put(game->mlx, game->win, 80, h + y_offset, col, out);
	free(out);
}

static void	print_game_status(t_game *game)
{
	int	r;
	int	c;

	c = 0;
	while (c < game->map_w)
	{
		r = c * ASSET_SIZE;
		mlx_put_image_to_window(game->mlx, game->win, game->i.deb, r,
			game->map_h * ASSET_SIZE);
		c++;
	}
	print_score(game, game->moves, "Moves:", 10);
	print_score(game, game->colls, "Colls left:", 30);
	if (game->msg_out != NULL)
		mlx_string_put(game->mlx, game->win, 110, game->map_h * ASSET_SIZE + 10,
			0x00FFFFFF, game->msg_out);
}

int	out(char *s1, char *s2, t_game *game)
{
	if (s1 != NULL)
	{
		if (game->msg_out != NULL)
			free(game->msg_out);
		clock_gettime(CLOCK_MONOTONIC, game->msg_clock);
		if (s2 != NULL)
			game->msg_out = ft_strjoin(s1, s2);
		else
			game->msg_out = strdp(s1);
		if (!game->msg_out)
			c_throw(game, NULL, NULL);
	}
	print_game_status(game);
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	return (0);
}

int	err(char *err_1, char *err_2)
{
	putstr_fd("Error\n", 2);
	if (err_1)
	{
		putstr_fd(err_1, 2);
		free(err_1);
	}
	if (err_2)
	{
		putstr_fd(err_2, 2);
		free(err_2);
		putstr_fd("\n", 2);
	}
	return (-1);
}

void	c_throw(t_game *game, char *err_1, char *err_2)
{
	t_game	*gm;

	gm = NULL;
	if (!err_1)
		err_1 = strdp("Generic Error\0");
	if (!err_2)
		err_2 = c_strerror();
	err(err_1, err_2);
	if (game && game->is_safe && game->ancestor)
	{
		gm = (t_game *)(game->ancestor);
		free_safe_game(game);
	}
	else if (game)
		gm = game;
	if (gm)
		free_game(gm);
	exit(1);
}
