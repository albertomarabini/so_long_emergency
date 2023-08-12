/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_init_mlx_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:24:07 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/12 00:29:41 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

static void	*l_img(void *mlx, char *path, int s)
{
	void	*img;
	int		w;
	int		h;

	w = s;
	h = s;
	img = mlx_xpm_file_to_image(mlx, path, &w, &h);
	if (!img)
	{
		err(strdup("Unable to load image "), c_strerror());
		return (NULL);
	}
	return (img);
}

static int	load_images(t_game *g)
{
	int	s;

	s = ASSET_SIZE;
	g->i.w_tl = l_img(g->mlx, "/root/C/so_long/assets/w_tl.xpm", s);
	g->i.w_tr = l_img(g->mlx, "/root/C/so_long/assets/w_tr.xpm", s);
	g->i.w_bl = l_img(g->mlx, "/root/C/so_long/assets/w_bl.xpm", s);
	g->i.w_br = l_img(g->mlx, "/root/C/so_long/assets/w_br.xpm", s);
	g->i.w_l = l_img(g->mlx, "/root/C/so_long/assets/w_l.xpm", s);
	g->i.w_r = l_img(g->mlx, "/root/C/so_long/assets/w_r.xpm", s);
	g->i.w_t = l_img(g->mlx, "/root/C/so_long/assets/wall.xpm", s);
	g->i.w_b = l_img(g->mlx, "/root/C/so_long/assets/wall.xpm", s);
	g->i.obst = l_img(g->mlx, "/root/C/so_long/assets/obst.xpm", s);
	g->i.coll = l_img(g->mlx, "/root/C/so_long/assets/key.xpm", s);
	g->i.exit = l_img(g->mlx, "/root/C/so_long/assets/door.xpm", s);
	g->i.hero = l_img(g->mlx, "/root/C/so_long/assets/hero.xpm", s);
	g->i.floor = l_img(g->mlx, "/root/C/so_long/assets/floor.xpm", s);
	g->i.vill = l_img(g->mlx, "/root/C/so_long/assets/vill.xpm", s);
	g->i.deb = l_img(g->mlx, "/root/C/so_long/assets/deb.xpm", s);
	if (!g->i.w_tl || !g->i.w_tr || !g->i.w_bl || !g->i.w_br || !g->i.w_l
		|| !g->i.w_r || !g->i.w_t || !g->i.w_b || !g->i.obst || !g->i.coll
		|| !g->i.exit || !g->i.hero || !g->i.floor || !g->i.vill || !g->i.deb)
		return (-1);
	return (0);
}

static void	draw_wall(int r, int c, t_game *game)
{
	if (c == 0 && r == 0)
		p_img(game, game->i.w_tl, r, c);
	else if (c == game->map_w - 1 && r == 0)
		p_img(game, game->i.w_tr, r, c);
	else if (c == 0 && r == game->map_h - 1)
		p_img(game, game->i.w_bl, r, c);
	else if (c == game->map_w - 1 && r == game->map_h - 1)
		p_img(game, game->i.w_br, r, c);
	else if (c == 0 && r > 0 && r < game->map_h - 1)
		p_img(game, game->i.w_l, r, c);
	else if (c == game->map_w - 1 && r > 0 && r < game->map_h - 1)
		p_img(game, game->i.w_r, r, c);
	else if (r == 0 || r == game->map_h - 1)
		p_img(game, game->i.w_t, r, c);
	else
		p_img(game, game->i.obst, r, c);
}

static void	render_map(t_game *game)
{
	int	r;
	int	c;

	mlx_clear_window(game->mlx, game->win);
	r = 0;
	while (r < game->map_h)
	{
		c = 0;
		while (c < game->map_w)
		{
			p_img(game, game->i.floor, r, c);
			if (game->map[r][c] == '1')
				draw_wall(r, c, game);
			else if (game->map[r][c] == 'C')
				p_img(game, game->i.coll, r, c);
			else if (game->map[r][c] == 'E')
				p_img(game, game->i.exit, r, c);
			else if (game->map[r][c] == 'V')
				p_img(game, game->i.vill, r, c);
			else if (game->map[r][c] == 'P')
				p_img(game, game->i.hero, r, c);
			c++;
		}
		r++;
	}
}

int	init_mlx(t_game *game)
{
	int	w;
	int	h;

	game->mlx = mlx_init();
	if (!game->mlx)
		return (err(strdup("Can't instantiate mlx: "), c_strerror()));
	mlx_get_screen_size(game->mlx, &w, &h);
	if (game->map_h * ASSET_SIZE + DEBUGGER_H > h || game->map_w
		* ASSET_SIZE > w)
		return (err(strdup("Map too big for screen\n"), NULL));
	w = game->map_w * ASSET_SIZE;
	h = game->map_h * ASSET_SIZE + DEBUGGER_H;
	game->win = mlx_new_window(game->mlx, w, h, "sl");
	if (!game->win)
		return (err(strdup("Can't instantiate mlx win: "), c_strerror()));
	if (load_images(game) == -1)
		return (err(strdup("Can't load images\n"), NULL));
	render_map(game);
	mlx_key_hook(game->win, on_keypress, game);
	// mlx_expose_hook(game->win, mlx_loop_end, game->mlx);
	mlx_destroy_hook(game->mlx, game->win);
	// mlx_loop_hook ( void *mlx_ptr, int (*funct_ptr)(), void *param );
	clock_gettime(CLOCK_MONOTONIC, game->hero_clock);
	clock_gettime(CLOCK_MONOTONIC, game->vill_clock);
	mlx_loop_hook(game->mlx, render_next_frame, game);
	mlx_loop(game->mlx);
	return (0);
}
