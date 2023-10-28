/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_mlx_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 10:33:02 by amarabin          #+#    #+#             */
/*   Updated: 2023/10/28 11:02:45 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	mlx_loop_end(void *mlx_ptr)
{
	((mlx_ptr_t *)mlx_ptr)->main_loop_active = 1;
	return (1);
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
