/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_init_mlx.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:24:07 by amarabin          #+#    #+#             */
/*   Updated: 2023/11/08 02:21:13 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

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
		err(strdup("Unable to load image "), strerror(errno));
		return (NULL);
	}
	return (img);
}

/**
 * This version is used for debugging purposes
 */
// static int	load_images(t_game *g)
// {
// 	int	s;

// 	s = ASSET_SIZE;
// 	g->imgs.w_tl = l_img(g->mlx,
// 			"/Users/amarabin/Development/DEV/so_long/assets/w_tl.xpm", s);
// 	g->imgs.w_tr = l_img(g->mlx,
// 			"/Users/amarabin/Development/DEV/so_long/assets/w_tr.xpm", s);
// 	g->imgs.w_bl = l_img(g->mlx,
// 			"/Users/amarabin/Development/DEV/so_long/assets/w_bl.xpm", s);
// 	g->imgs.w_br = l_img(g->mlx,
// 			"/Users/amarabin/Development/DEV/so_long/assets/w_br.xpm", s);
// 	g->imgs.w_l = l_img(g->mlx,
// 			"/Users/amarabin/Development/DEV/so_long/assets/w_l.xpm", s);
// 	g->imgs.w_r = l_img(g->mlx,
// 			"/Users/amarabin/Development/DEV/so_long/assets/w_r.xpm", s);
// 	g->imgs.w_t = l_img(g->mlx,
// 			"/Users/amarabin/Development/DEV/so_long/assets/w_t.xpm", s);
// 	g->imgs.w_b = l_img(g->mlx,
// 			"/Users/amarabin/Development/DEV/so_long/assets/w_b.xpm", s);
// 	g->imgs.obst = l_img(g->mlx,
// 			"/Users/amarabin/Development/DEV/so_long/assets/obst.xpm", s);
// 	g->imgs.coll = l_img(g->mlx,
// 			"/Users/amarabin/Development/DEV/so_long/assets/key.xpm", s);
// 	g->imgs.exit = l_img(g->mlx,
// 			"/Users/amarabin/Development/DEV/so_long/assets/door.xpm", s);
// 	g->imgs.hero = l_img(g->mlx,
// 			"/Users/amarabin/Development/DEV/so_long/assets/hero.xpm", s);
// 	g->imgs.floor = l_img(g->mlx,
// 			"/Users/amarabin/Development/DEV/so_long/assets/floor.xpm", s);
// 	if (!g->imgs.w_tl || !g->imgs.w_tr || !g->imgs.w_bl || !g->imgs.w_br
// 		|| !g->imgs.w_l || !g->imgs.w_r || !g->imgs.w_t || !g->imgs.w_b
// 		|| !g->imgs.obst || !g->imgs.coll || !g->imgs.exit || !g->imgs.hero
// 		|| !g->imgs.floor)
// 		return (-1);
// 	return (0);
// }

static int	load_images(t_game *g)
{
	int	s;

	s = ASSET_SIZE;
	g->imgs.w_tl = l_img(g->mlx, "./assets/w_tl.xpm", s);
	g->imgs.w_tr = l_img(g->mlx, "./assets/w_tr.xpm", s);
	g->imgs.w_bl = l_img(g->mlx, "./assets/w_bl.xpm", s);
	g->imgs.w_br = l_img(g->mlx, "./assets/w_br.xpm", s);
	g->imgs.w_l = l_img(g->mlx, "./assets/w_l.xpm", s);
	g->imgs.w_r = l_img(g->mlx, "./assets/w_r.xpm", s);
	g->imgs.w_t = l_img(g->mlx, "./assets/w_t.xpm", s);
	g->imgs.w_b = l_img(g->mlx, "./assets/w_b.xpm", s);
	g->imgs.obst = l_img(g->mlx, "./assets/obst.xpm", s);
	g->imgs.coll = l_img(g->mlx, "./assets/key.xpm", s);
	g->imgs.exit = l_img(g->mlx, "./assets/door.xpm", s);
	g->imgs.hero = l_img(g->mlx, "./assets/hero.xpm", s);
	g->imgs.floor = l_img(g->mlx, "./assets/floor.xpm", s);
	if (!g->imgs.w_tl || !g->imgs.w_tr || !g->imgs.w_bl || !g->imgs.w_br
		|| !g->imgs.w_l || !g->imgs.w_r || !g->imgs.w_t || !g->imgs.w_b
		|| !g->imgs.obst || !g->imgs.coll || !g->imgs.exit || !g->imgs.hero
		|| !g->imgs.floor)
		return (-1);
	return (0);
}

static void	draw_wall(int r, int c, t_game *game)
{
	if (c == 0 && r == 0)
		p_img(game, game->imgs.w_tl, r, c);
	else if (c == game->map_w - 1 && r == 0)
		p_img(game, game->imgs.w_tr, r, c);
	else if (c == 0 && r == game->map_h - 1)
		p_img(game, game->imgs.w_bl, r, c);
	else if (c == game->map_w - 1 && r == game->map_h - 1)
		p_img(game, game->imgs.w_br, r, c);
	else if (c == 0 && r > 0 && r < game->map_h - 1)
		p_img(game, game->imgs.w_l, r, c);
	else if (c == game->map_w - 1 && r > 0 && r < game->map_h - 1)
		p_img(game, game->imgs.w_r, r, c);
	else if (r == 0)
		p_img(game, game->imgs.w_t, r, c);
	else if (r == game->map_h - 1)
		p_img(game, game->imgs.w_b, r, c);
	else
		p_img(game, game->imgs.obst, r, c);
}

static void	render_map(t_game *game)
{
	int	r;
	int	c;

	r = 0;
	while (r < game->map_h)
	{
		c = 0;
		while (c < game->map_w)
		{
			p_img(game, game->imgs.floor, r, c);
			if (game->map[r][c] == '1')
				draw_wall(r, c, game);
			else if (game->map[r][c] == 'C')
				p_img(game, game->imgs.coll, r, c);
			else if (game->map[r][c] == 'E')
				p_img(game, game->imgs.exit, r, c);
			else if (game->map[r][c] == 'P')
			{
				p_img(game, game->imgs.hero, r, c);
				game->map[r][c] = '0';
			}
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
		return (err(strdup("Can't instantiate mlx: "), strerror(errno)));
	mlx_get_screen_size(game->mlx, &w, &h);
	if (game->map_h * ASSET_SIZE > w || game->map_w * ASSET_SIZE > h)
		return (err(strdup("Map too big for screen\n"), NULL));
	if (game->map_h < 3 || game->map_w < 3)
		return (err(strdup("Unplayable Map\n"), NULL));
	w = game->map_w * ASSET_SIZE;
	h = game->map_h * ASSET_SIZE;
	game->win = mlx_new_window(game->mlx, w, h, "sl");
	if (!game->win)
		return (err(strdup("Can't instantiate mlx win: "), strerror(errno)));
	if (load_images(game) == -1)
		return (err(strdup("Can't load images\n"), NULL));
	mlx_clear_window(game->mlx, game->win);
	render_map(game);
	mlx_key_hook(game->win, on_keypress, game);
	mlx_destroy_hook(game->win, game);
	mlx_loop(game->mlx);
	return (0);
}

int	is_map_unreacheable(int r, int c, t_game eval, t_game *game)
{
	if (r < 0 || c < 0 || r > eval.map_h - 1 || c > eval.map_w - 1)
		return (0);
	if (r == game->hero_r && c == game->hero_c)
	{
		eval.colls = 0;
		eval.exit = 0;
	}
	if (game->map[r][c] == '1')
		return (0);
	if (game->map[r][c] == 'E' && eval.exit < game->exit)
		eval.exit++;
	if (game->map[r][c] == 'C' && eval.colls < game->colls)
		eval.colls++;
	is_map_unreacheable(r + 1, c, eval, game);
	is_map_unreacheable(r - 1, c, eval, game);
	is_map_unreacheable(r, c + 1, eval, game);
	is_map_unreacheable(r, c - 1, eval, game);
	if (r == game->hero_r && c == game->hero_c)
	{
		if (eval.exit == game->exit && eval.colls == game->colls)
			return (1);
		else
			return (0);
	}
	return (0);
}
