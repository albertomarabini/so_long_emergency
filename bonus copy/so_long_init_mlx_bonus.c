/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_init_mlx_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:24:07 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/17 11:19:00 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

static int	load_images(t_game *g)
{
	int	s;

	s = ASSET_SIZE;
	g->i.w_tl = l_img(g->mlx, ASTS_PATH, "w_tl.xpm", s);
	g->i.w_tr = l_img(g->mlx, ASTS_PATH, "w_tr.xpm", s);
	g->i.w_bl = l_img(g->mlx, ASTS_PATH, "w_bl.xpm", s);
	g->i.w_br = l_img(g->mlx, ASTS_PATH, "w_br.xpm", s);
	g->i.w_l = l_img(g->mlx, ASTS_PATH, "w_l.xpm", s);
	g->i.w_r = l_img(g->mlx, ASTS_PATH, "w_r.xpm", s);
	g->i.w_t = l_img(g->mlx, ASTS_PATH, "w_t.xpm", s);
	g->i.w_b = l_img(g->mlx, ASTS_PATH, "w_b.xpm", s);
	g->i.obst = l_img(g->mlx, ASTS_PATH, "obst.xpm", s);
	g->i.coll = l_img(g->mlx, ASTS_PATH, "key.xpm", s);
	g->i.exit = l_img(g->mlx, ASTS_PATH, "door.xpm", s);
	g->i.hero = l_img(g->mlx, ASTS_PATH, "hero.xpm", s);
	g->i.floor = l_img(g->mlx, ASTS_PATH, "floor.xpm", s);
	g->i.vill = l_img(g->mlx, ASTS_PATH, "vill.xpm", s);
	g->i.deb = l_img(g->mlx, ASTS_PATH, "deb.xpm", s);
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
	else if (r == 0)
		p_img(game, game->i.w_t, r, c);
	else if (r == game->map_h - 1)
		p_img(game, game->i.w_b, r, c);
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
		while (game->map_w > c++)
		{
			p_img(game, game->i.floor, r, c - 1);
			if (game->map[r][c - 1] == '1')
				draw_wall(r, c - 1, game);
			else if (game->map[r][c - 1] == 'C')
				p_img(game, game->i.coll, r, c - 1);
			else if (game->map[r][c - 1] == 'E')
				p_img(game, game->i.exit, r, c - 1);
			else if (game->map[r][c - 1] == 'P')
				game->map[r][c - 1] = '0';
		}
		r++;
	}
}

int	init_mlx(t_game *gm)
{
	int	i;

	gm->mlx = mlx_init();
	if (!gm->mlx)
		return (err(strdup("Can't instantiate mlx: "), c_strerror()));
	if (intantiate_mlx_window(gm, W_NAME) == -1)
		return (-1);
	i = 0;
	gm->vill_spt = (t_stp **)c_alloc(gm, sizeof(t_stp *) * (gm->vills + 1));
	while (gm->vills > i++)
		gm->vill_spt[i - 1] = init_sprite(gm, create_p((i % 2), 2, 0, 0),
				strdup(V_ASTS_PATH), strdup(".xpm"));
	gm->vill_spt[gm->vills] = NULL;
	gm->hero_spt = init_sprite(gm, create_p(0, 4, 0, 0), strdup(H_ASTS_PATH),
			strdup(".xpm"));
	if (load_images(gm) == -1)
		return (err(strdup("Can't load images\n"), NULL));
	render_map(gm);
	mlx_key_hook(gm->win, on_keypress, gm);
	mlx_destroy_hook(gm->mlx, gm->win);
	clock_gettime(CLOCK_MONOTONIC, gm->hero_clock);
	clock_gettime(CLOCK_MONOTONIC, gm->vill_clock);
	mlx_loop_hook(gm->mlx, render_next_frame, gm);
	mlx_loop(gm->mlx);
	return (0);
}
