/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:24:07 by amarabin          #+#    #+#             */
/*   Updated: 2023/11/13 17:13:31 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

static void	unreach(int r, int c, t_game eval, char **map)
{
	if (r < 0 || c < 0 || r > eval.map_h - 1 || c > eval.map_w - 1)
		return ;
	if (map[r][c] == '1')
		return ;
	if (map[r][c] == 'E')
		eval.exits--;
	if (map[r][c] == 'C')
		eval.colls--;
	map[r][c] = '1';
	unreach(r + 1, c, eval, map);
	unreach(r - 1, c, eval, map);
	unreach(r, c + 1, eval, map);
	unreach(r, c - 1, eval, map);
}

static int	test_unreach(t_game *gm)
{
	t_game	eval;
	char	**map;
	int 	i;

	eval = *gm;
	map = duplicate_map(gm);
	if (!map)
		return (0);
	unreach(eval.hero_r, eval.hero_c, eval, map);
	i = 0;
	while (i < eval.map_h)
	{
		if (map[i])
			free(map[i]);
		i++;
	}
	if (eval.exits != 0 || eval.colls != 0)
		return (0);
	return (1);
}

/**
 * This function validates the game map by performing the following checks:
 * 1. The dimensions of the map are at least 3 on each side. If not, the
 *    map is deemed unplayable.
 * 2. The outermost rows of the map (first and last) are checked to
 *    ensure they contains all 1s.
 * 3. The internal cells are validated using the `validate_map_line` function,
 *    that will validate the content of the map line by line and globally
 * 4. After iterating through the entire map, the function checks that there
 *    is at least one exit and one collectible.
 * 5. Checks if each collectible and exit is reachable.
 * @param gm The game state containing the map and other game-related data.
 * @return Returns 0 if the map is valid.
 */
static int	validate_map(t_game *gm)
{
	if (gm->map_h < 3 || gm->map_w < 3)
		return (err(strdp("Unplayable Map\n"), NULL));
	if (validate_map_content(gm) == -1)
		return (-1);
	if (!gm->exits || !gm->colls || !gm->hero || !gm->vills)
		return (err(strdp("No exit, hero, villain or collectible\n"), NULL));
	if (!test_unreach(gm))
		return (err(strdp("Unreachable cells\n"), NULL));
	gm->a_vills = (t_point **)c_alloc(gm, sizeof(t_point *) * (gm->vills + 1));
	gm->a_colls = (t_point **)c_alloc(gm, sizeof(t_point *) * (gm->colls + 1));
	gm->a_exits = (t_point **)c_alloc(gm, sizeof(t_point *) * (gm->exits + 1));
	populate_tgt_arrays(gm);
	gm->a_vills[gm->vills] = NULL;
	gm->a_colls[gm->colls] = NULL;
	gm->a_exits[gm->exits] = NULL;
	gm->o_map = duplicate_map(gm);
	gm->a_corners = find_corners(gm);
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
static int	init_game_map(t_game *gm, char *last_line)
{
	int	i;

	gm->map = (char **)malloc(sizeof(char *) * (gm->map_h + 1));
	if (!gm->map)
		return (err(strdp("Unable to instantiate map\n"), c_strerror()));
	i = 0;
	while (i <= gm->map_h)
	{
		gm->map[i] = NULL;
		i++;
	}
	gm->map[gm->map_h - 1] = last_line;
	return (gm->map_h - 1);
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
 * there is no need to free the variable char *line since each line instance
 * generated is stored in the game map.
 */
int	read_map(t_game *game, int fd)
{
	char	*line;
	int		res;

	res = get_next_line(fd, &line);
	if (res == -1)
		return (err(strdp("Error while reading map: "), c_strerror()));
	game->map_h++;
	if (game->map_w < 0)
		game->map_w = (int)strl(line);
	if (game->map_w != (int)strl(line) || game->map_w == 0)
		return (err(strdp("Uneven or empty row at "), itoa(game->map_h)));
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
