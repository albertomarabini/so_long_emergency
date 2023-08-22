/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:41:20 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/22 17:25:40 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

t_point	hero_p(t_game *gm)
{
	t_point	p;

	p.r = gm->hero_r;
	p.c = gm->hero_c;
	p.val = 0;
	p.val2 = 0;
	return (p);
}

static void	fill_map_unsafe_spots(t_game *gm, char **map, t_point p, int ray)
{
	if (strchr("1E", map[p.r][p.c]) || (p.r == gm->hero_r && p.c == gm->hero_c)
		|| ray == 0)
		return ;
	map[p.r][p.c] = '1';
	fill_map_unsafe_spots(gm, map, create_p(p.r - 1, p.c, 0, 0), ray - 1);
	fill_map_unsafe_spots(gm, map, create_p(p.r + 1, p.c, 0, 0), ray - 1);
	fill_map_unsafe_spots(gm, map, create_p(p.r, p.c - 1, 0, 0), ray - 1);
	fill_map_unsafe_spots(gm, map, create_p(p.r, p.c + 1, 0, 0), ray - 1);
}

static void	create_safe_map(t_game *gm, t_game *safe, int safe_w)
{
	int	r;
	int	c;

	safe->map = duplicate_map(gm);
	r = 0;
	while (gm->map_h > r++)
	{
		c = 0;
		while (gm->map_w > c++)
			if (safe->map[r - 1][c - 1] == 'V')
				fill_map_unsafe_spots(gm, safe->map, create_p(r - 1, c - 1, 0,
						0), safe_w);
	}
}

t_game	*create_safe_game(t_game *gm, int safe_w)
{
	t_game	*safe;

	safe = (t_game *)c_alloc(gm, sizeof(t_game));
	create_safe_map(gm, safe, safe_w);
	safe->is_safe = 1;
	safe->ancestor = (void *)gm;
	safe->map_h = gm->map_h;
	safe->map_w = gm->map_w;
	safe->hero_c = gm->hero_c;
	safe->hero_r = gm->hero_r;
	safe->a_corners = gm->a_corners;
	safe->a_vills = gm->a_vills;
	safe->a_colls = gm->a_colls;
	safe->a_exits = gm->a_exits;
	safe->vills = gm->vills;
	safe->colls = gm->colls;
	safe->exits = gm->exits;
	null_a(safe->tgt_el_q, TGT_Q_LEN);
	return (safe);
}

// DEBUGGING FUNCTIONS
// static void print_map(char **o_map) {
//     if (o_map == NULL) {
//         printf("The map is NULL.\n");
//         return ;
//     }

//     for (int i = 0; o_map[i] != NULL; i++) {
//         printf("%s\n", o_map[i]);
//     }
// }
