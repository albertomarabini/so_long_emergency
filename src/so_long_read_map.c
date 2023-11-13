/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_read_map.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:24:07 by amarabin          #+#    #+#             */
/*   Updated: 2023/11/12 23:16:48 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/**
 * Validates a single row of the game map (cell by cell excluding the first and
 * last rows) while gathering global infos on the map.
 * The following checks are made:
 * 1. If contains only the symbols '0', 'P', 'E', 'C', or '1'.
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
static int	val_line(t_game *game, int r, int c)
{
	if (!strch("0PEC1", game->map[r][c]))
		return (err(strdp("Invalid symbol at "), xyta(r, c)));
	if (game->map[r][c] == 'P')
	{
		if (game->hero > 0)
			return (err(strdp("Multiple heroes\n"), NULL));
		game->hero++;
		game->hero_r = r;
		game->hero_c = c;
		game->map[r][c] = 'P';
	}
	else if (game->map[r][c] == 'C')
		game->colls++;
	else if (game->map[r][c] == 'E')
		game->exit++;
	if ((c == 0 || c == game->map_w - 1) && game->map[r][c] != '1')
		return (err(strdp("Invalid symbol at "), xyta(r, c)));
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

	r = 0;
	while (r < game->map_h)
	{
		c = 0;
		while (c < game->map_w)
		{
			if ((r > 0 && r < game->map_h - 1) && val_line(game, r, c) == -1)
				return (-1);
			if ((r == 0 || r == game->map_h - 1) && game->map[r][c] != '1')
				return (err(strdp("Invalid symbol at "), xyta(r, c)));
			c++;
		}
		r++;
	}
	if (!game->exit || !game->colls || !game->hero)
		return (err(strdp("No exits, hero or collectibles\n"), NULL));
	if (!test_unreach(game))
		return (err(strdp("Unreachable cells\n"), NULL));
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

	game->map = (char **)malloc(sizeof(char *) * (game->map_h + 1));
	if (!game->map)
		return (err(strdp("Unable to instantiate map\n"), strerror(errno)));
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
 * there is no need to free the variable char *line since each line instance
 * generated is stored in the game map.
 */
int	read_map(t_game *game, int fd)
{
	char	*line;
	int		res;

	res = get_next_line(fd, &line);
	if (res == -1)
		return (err(strdp("Error while reading map: "), strerror(errno)));
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
