/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 05:17:54 by amarabin          #+#    #+#             */
/*   Updated: 2023/11/12 19:49:05 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

/**
 * Prints an image in the correct position on the window.
 */
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
int	mlx_destroy_hook(t_game *gm, void *win_ptr)
{
	return (mlx_hook(win_ptr, 17, 0, free_game, gm));
}

/**
 * Given a file path, loads an image into memory.
 */
void	*l_img(void *mlx, char *path, char *img, int s)
{
	void	*o_img;
	char	*c_path;
	int		w;
	int		h;

	c_path = ft_strjoin(path, img);
	w = s;
	h = s;
	o_img = mlx_xpm_file_to_image(mlx, c_path, &w, &h);
	if (!o_img)
	{
		err(strdp("Unable to load image "), c_strerror());
		return (NULL);
	}
	free(c_path);
	return (o_img);
}

int	intantiate_mlx_window(t_game *gm, char *name)
{
	int	w;
	int	h;

	mlx_get_screen_size(gm->mlx, &w, &h);
	if (gm->map_h * ASSET_SIZE + DEBUGGER_H > h || gm->map_w * ASSET_SIZE > w)
		return (err(strdp("Map too big for screen \n"), NULL));
	w = gm->map_w * ASSET_SIZE;
	h = gm->map_h * ASSET_SIZE + DEBUGGER_H;
	gm->win = mlx_new_window(gm->mlx, w, h, name);
	if (!gm->win)
		return (err(strdp("Can't instantiate mlx win: "), c_strerror()));
	return (0);
}
