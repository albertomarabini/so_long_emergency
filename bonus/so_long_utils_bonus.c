/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:41:20 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/03 23:08:57 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

static void	err_out(char *s1, char *s2, int fd)
{
	if (s1)
	{
		putstr_fd(s1, fd);
		free(s1);
	}
	if (s2)
	{
		putstr_fd(s2, fd);
		free(s2);
		putstr_fd("\n", fd);
	}
}

int	out(char *s1, char *s2, t_game *game)
{
	char	*out;
	int		i;
	int		r;
	int		c;

	i = 0;
	if (s2 != NULL)
		out = ft_strjoin(s1, s2);
	else
		out = strdup(s1);
	while (i < game->map_w)
	{
		r = i * DEBUGGER_H;
		c = game->map_h * DEBUGGER_H;
		mlx_put_image_to_window(game->mlx, game->win, game->i.deb, r, c);
		i++;
	}
	mlx_string_put(game->mlx, game->win, 5, game->map_h * ASSET_SIZE + 10,
		0xFFFFFF, out);
	free(out);
	free(s1);
	if (s2)
		free(s2);
	return (0);
}

int	err(char *err_1, char *err_2)
{
	putstr_fd("Error\n", 2);
	err_out(err_1, err_2, 2);
	return (-1);
}

void	p_img(t_game *game, void *img, int r, int c)
{
	r *= ASSET_SIZE;
	c *= ASSET_SIZE;
	mlx_put_image_to_window(game->mlx, game->win, img, c, r);
}

/**
 * Associates the closing of the window with ending the MLX loop by the
 * mlx_hook function.
 * The event code (17) and event mask (0) are specific to the X11 events system.
 * 17 corresponds to the DestroyNotify event, which is triggered when the
 * window is about to be destroyed
 * The function mlx_loop_end is an internal function of minilibx that
 * stops the MLX loop.
 *
 * @param mlx_ptr A pointer to the MLX connection instance.
 * @param win_ptr A pointer to the MLX window instance.
 * @param funct_ptr A function pointer that will be triggered
 * when the event occurs.
 * @param param An additional parameter that will be passed to the
 * funct_ptr when it is called.
 *
 */
int	mlx_destroy_hook(void *mlx_ptr, void *win_ptr)
{
	return (mlx_hook(win_ptr, 17, 0, mlx_loop_end, mlx_ptr));
}
