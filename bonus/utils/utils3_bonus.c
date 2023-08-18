/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:41:20 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/17 05:17:44 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

static void	print_on_debugger(t_game *game)
{
	int		r;
	int		c;
	char	*out;

	c = 0;
	while (c < game->map_w)
	{
		r = c * ASSET_SIZE;
		mlx_put_image_to_window(game->mlx, game->win, game->i.deb, r,
			game->map_h * ASSET_SIZE);
		c++;
	}
	c = 0x00FFFFFF;
	r = game->map_h * ASSET_SIZE;
	out = itoa(game->moves);
	mlx_string_put(game->mlx, game->win, 10, r + 10, c, "Moves: ");
	mlx_string_put(game->mlx, game->win, 80, r + 10, c, out);
	free(out);
	out = itoa(game->colls);
	mlx_string_put(game->mlx, game->win, 10, r + 30, c, "Colls left: ");
	mlx_string_put(game->mlx, game->win, 80, r + 30, c, out);
	free(out);
	if (game->msg_out != NULL)
		mlx_string_put(game->mlx, game->win, 110, r + 10, c, game->msg_out);
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
			game->msg_out = strdup(s1);
	}
	print_on_debugger(game);
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
