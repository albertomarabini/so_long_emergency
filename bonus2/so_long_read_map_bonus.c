/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_read_map_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:24:07 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/12 00:29:41 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"


t_point	*fnd_crn(t_point *ij, t_game *gm)
{
	t_point	rc;

	rc.r = trn(ij->r, gm->map_h - 2, 1);
	rc.c = trn(ij->c, gm->map_w - 2, 1);
	while ((!ij->c && rc.c < gm->map_w / 2) || (ij->c && rc.c > gm->map_w / 2))
	{
		if (strchr("E0C", gm->map[rc.r][rc.c]))
			return (create_p2(rc.r, rc.c, 0, 42));
		rc.c += trn(ij->c, -1, +1);
		if (rc.c == gm->map_w / 2)
		{
			rc.r += trn(ij->r, -1, +1);
			rc.c = trn(ij->c, gm->map_w - 2, 1);
			if (rc.r == gm->map_h)
				return (NULL);
		}
	}
}

t_point	**find_corners(t_game *gm)
{
	t_point	**corners;
	t_point	*ij;
	int		corner;

	corners = c_alloc(5 * sizeof(t_point *));
	ij = create_p(0, 0, 0);
	corner = 0;
	while (ij->r < 2)
	{
		ij->c = 0;
		while (ij->c < 2)
		{
			corners[corner++] = fnd_crn(ij, gm);
			if (corners[corner - 1] == NULL)
			{
				c_frr("", ij);
				return (NULL);
			}
			ij->c++;
		}
		ij->r++;
	}
	corners[4] = NULL;
	c_frr("", ij);
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
		game->map[r][c] = '0';
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

/**
 * This function validates the game map by performing the following checks:
 * 1. The dimensions of the map are at least 4x4. If not, the map is deemed
 *    unplayable.
 * 2. The outermost rows of the map (first and last) are checked to
 *    ensure they contains all 1s.
 * 3. The internal cells are validated using the `validate_map_line` function,
 *    that will validate the content of the map line by line and globally
 * 4. After iterating through the entire map, the function checks that there
 *    is at least one exit and one collectible.
 * @param game The game state containing the map and other game-related data.
 * @return Returns 0 if the map is valid.
 */
static int	validate_map(t_game *game)
{
	int	r;
	int	c;
	int	vill_c;
	int	coll_c;
	int	exit_c;

	if (game->map_h < 3 || game->map_w < 3)
		return (err(strdup("Unplayable Map\n"), NULL));
	r = 0;
	while (r < game->map_h)
	{
		c = 0;
		while (c < game->map_w)
		{
			if ((r > 0 || r < game->map_h - 1) && vld_line(game, r, c) == -1)
				return (-1);
			if ((r == 0 || r == game->map_h - 1) && game->map[r][c] != '1')
				return (err(strdup("Invalid symbol at "), xyta(r, c)));
			c++;
		}
		r++;
	}
	if (!game->exits || !game->colls || !game->hero || !game->vills)
		return (err(strdup("No exit, hero, villain or collectible\n"), NULL));
	game->a_vills = (t_point **)c_alloc(sizeof(t_point *) * (game->vills + 1));
	game->a_colls = (t_point **)c_alloc(sizeof(t_point *) * (game->colls + 1));
	game->a_exits = (t_point **)c_alloc(sizeof(t_point *) * (game->exits + 1));
	vill_c = 0;
	coll_c = 0;
	exit_c = 0;
	r = 1;
	while (r < game->map_h - 1)
	{
		c = 1;
		while (c < game->map_w - 1)
		{
			if (game->map[r][c] == 'V')
				game->a_vills[vill_c++] = create_p(r, c, 0);
			if (game->map[r][c] == 'C')
				game->a_colls[coll_c++] = create_p(r, c, 0);
			if (game->map[r][c] == 'E')
				game->a_exits[exit_c++] = create_p(r, c, 0);
			c++;
		}
		r++;
	}
	game->a_vills[vill_c] = NULL;
	game->a_colls[coll_c] = NULL;
	game->a_exits[exit_c] = NULL;
	game->o_map = duplicate_map(game);
	game->a_corners = find_corners(game);
	return (0);
}

/**
 * Once reached the bottom of the map we can instantiate the map variable
 * accordingly and add the last read from the file.
 *
 * @param game A pointer to the t_game structure where the game's details are
 * stored.
 * @param lastline The last read from the file.
 */
static int	init_game_map(t_game *game, char *last_line)
{
	int	i;

	game->map = (char **)c_alloc(sizeof(char *) * (game->map_h + 1));
	if (!game->map)
		return (err(strdup("Unable to instantiate map\n"), c_strerror()));
	i = 0;
	while (i <= game->map_h)
	{
		game->map[i] = NULL;
		i++;
	}
	game->map[game->map_h - 1] = last_line;
	return (game->map_h - 1);
}

/**
 * Reads and validates a map, ensuring the map's rows are all of equal length
 * and not empty. Uses recursion to process each line in the file: at the end
 * of the recursion phase ehen we know the dimensions of the matrix, we can
 * track our steps back adding each line in reverse up to the beginning of the
 * matrix. Then we can validate the map
 *
 * @param game A pointer to the t_game structure where the game's details are
 * stored.
 * @param fd The file descriptor from which the map is read.
 *
 * @return Returns -1 if there's an error during reading the map or if a row is
 * in regards of the other rows or if the map is empty.
 * there is no need to c_frr the variable char *line since each line instance
 * generated is stored in the game map.
 */
int	read_map(t_game *game, int fd)
{
	char	*line;
	int		res;

	res = get_next_line(fd, &line);
	if (res == -1)
		return (err(strdup("Error while reading map: "), c_strerror()));
	game->map_h++;
	if (game->map_w < 0)
		game->map_w = (int)strlen(line);
	if (game->map_w != (int)strlen(line) || game->map_w == 0)
		return (err(strdup("Uneven or empty row at "), itoa(game->map_h)));
	if (res == 1)
	{
		res = read_map(game, fd);
		if (res == -1)
			return (-1);
		game->map[res - 1] = line;
		if (res == 1)
			return (validate_map(game));
		return (res - 1);
	}
	return (init_game_map(game, line));
}
