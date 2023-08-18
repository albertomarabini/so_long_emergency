/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map2_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 04:35:17 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/17 04:40:06 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

static t_point	*fnd_crn(t_point ij, t_game *gm)
{
	t_point	rc;

	rc.r = trn(ij.r, gm->map_h - 2, 1);
	rc.c = trn(ij.c, gm->map_w - 2, 1);
	while ((!ij.c && rc.c < gm->map_w / 2) || (ij.c && rc.c > gm->map_w / 2))
	{
		if (strchr("E0C", gm->map[rc.r][rc.c]))
			return (create_pp(rc.r, rc.c, 0, 42));
		rc.c += trn(ij.c, -1, +1);
		if (rc.c == gm->map_w / 2)
		{
			rc.r += trn(ij.r, -1, +1);
			rc.c = trn(ij.c, gm->map_w - 2, 1);
			if (rc.r == gm->map_h)
				return (NULL);
		}
	}
	return (NULL);
}

t_point	**find_corners(t_game *gm)
{
	t_point	**corners;
	t_point	ij;
	int		corner;

	corners = (t_point **)c_alloc(gm, 5 * sizeof(t_point *));
	ij = create_p(0, 0, 0, 0);
	corner = 0;
	while (ij.r < 2)
	{
		ij.c = 0;
		while (ij.c < 2)
		{
			corners[corner++] = fnd_crn(ij, gm);
			if (corners[corner - 1] == NULL)
				return (NULL);
			ij.c++;
		}
		ij.r++;
	}
	corners[4] = NULL;
	return (corners);
}

/**
 * Validates a single row of the game map (cell by cell excluding the first and
 * last rows) while gathering global infos on the map.
 * The following checks are made:
 * 1. If contains only the symbols '0', 'P', 'E', 'C', 'V', or '1'.
 * 2. If 'P' is found (the hero) records the coordinates and checks whether
 * there was already one hero on the map.
 * 3. If 'C' is found, the count of collectibles is incremented.
 * 4. Ifs 'E', the count of exits is incremented.
 * 5. It checks whether the row boundaries contains '1'.
 *
 * @param game The game state containing the map and other game-related data.
 * @param r The row index of the cell to validate.
 * @param c The column index of the cell to validate.
 */
static int	vld_line(t_game *game, int r, int c)
{
	if (!strchr("0PECV1", game->map[r][c]))
		return (err(strdup("Invalid symbol at "), xyta(r, c)));
	if (game->map[r][c] == 'P')
	{
		if (game->hero > 0)
			return (err(strdup("Multiple heroes\n"), NULL));
		game->hero++;
		game->hero_r = r;
		game->hero_c = c;
		game->map[r][c] = 'P';
	}
	else if (game->map[r][c] == 'C')
		game->colls++;
	else if (game->map[r][c] == 'E')
		game->exits++;
	else if (game->map[r][c] == 'V')
		game->vills++;
	if ((c == 0 || c == game->map_w - 1) && game->map[r][c] != '1')
		return (err(strdup("Invalid symbol at "), xyta(r, c)));
	return (0);
}

int	validate_map_content(t_game *game)
{
	int	r;
	int	c;

	r = 0;
	while (r < game->map_h)
	{
		c = 0;
		while (c < game->map_w)
		{
			if ((r > 0 && r < game->map_h - 1) && vld_line(game, r, c) == -1)
				return (-1);
			if ((r == 0 || r == game->map_h - 1) && game->map[r][c] != '1')
				return (err(strdup("Invalid symbol at "), xyta(r, c)));
			c++;
		}
		r++;
	}
	return (0);
}

void	populate_tgt_arrays(t_game *gm)
{
	int	vill_c;
	int	coll_c;
	int	exit_c;
	int	r;
	int	c;

	vill_c = 0;
	coll_c = 0;
	exit_c = 0;
	r = 1;
	while (r < gm->map_h - 1)
	{
		c = 1;
		while (c < gm->map_w - 1)
		{
			if (gm->map[r][c] == 'V')
				gm->a_vills[vill_c++] = create_pp(r, c, 0, 0);
			if (gm->map[r][c] == 'C')
				gm->a_colls[coll_c++] = create_pp(r, c, 0, 0);
			if (gm->map[r][c] == 'E')
				gm->a_exits[exit_c++] = create_pp(r, c, 0, 0);
			c++;
		}
		r++;
	}
}
