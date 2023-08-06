/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_path_finder_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 10:47:19 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/01 00:08:18 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

char* mtostr(int **matrix, int rows, int cols) {
    // Calculate the length of the string.
    // Assuming that an integer takes up to 10 characters,
    // plus 2 for the space and the newline, plus 1 for the null terminator.
    int length = rows * (cols * (10 + 2) + 1) + 1;
    char* str = malloc(length);
    if (!str) {
        // handle allocation failure
        return NULL;
    }

    str[0] = '\0';  // Start with an empty string
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char buf[12];  // Buffer to hold one integer
			if (matrix[i][j] == INT_MAX)
				sprintf(buf, "%s ", "IM");
			else
            	sprintf(buf, "%2d ", matrix[i][j]);
            strcat(str, buf);  // Append to the string
        }
        strcat(str, "\n");  // Newline after each row
    }

    return str;
}

void	free_paths(int **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}

/**
 * creates a new row for paths initialized at INT_MAX
 */
static int	*init_paths_row(t_game *game, int r)
{
	int	*row;
	int	i;

	row = (int *)malloc(sizeof(int) * (game->map_w));
	if (!row)
		return (NULL);
	i = 0;
	while (i < game->map_w)
		row[i++] = INT_MAX;
	return (row);
}

/**
 * initializes a new matrix to investigate on all the possible paths vor the
 * villain to get to the hero.
 */
static int	**init_paths_matrix(t_game *game)
{
	int	**paths;
	int	r;

	paths = (int **)malloc(sizeof(int *) * (game->map_h + 1));
	if (!paths)
	{
		err(strdup("Unable to allocate memory"), strerror(errno));
		return (NULL);
	}
	r = 0;
	while (r < game->map_h)
	{
		paths[r++] = init_paths_row(game, r);
		if (!paths[r - 1])
		{
			err(strdup("Unable to allocate memory"), strerror(errno));
			free_paths(paths);
			return (NULL);
		}
	}
	paths[r] = NULL;
	return (paths);
}

/**
 * @brief This function computes all the possible paths to the current
 * position of the hero by recursively exploring all the directions
 *  from the position of the start point `p`.
 * The function stops the exploration if it reaches a point outside the map,
 * or if it encounters a point which it cannot pass
 * ('P', 'E', 'D', '1' characters),
 * or if the new step count `stp` is not less than the one recorded
 * by a different call to path_finder or if it reaches the hero's position.
 * Ita doesn't need to returm anything because hopefully by the time it's done
 * the paths matrix will be filled with the shortest path to the hero.
 *
 * For example, starting from the following map:
 * 	P _ _ _ _ _
 * 	_ _ _ _ O _
 * 	_ _ O _ _ _
 * 	_ _ _ _ _ _
 * 	_ O _ _ O _
 * 	_ _ _ _ _ V
 *
 * The final content of `paths` would be this:
 * 	P 9 8 7 6 5
 * 	9 8 7 6 O 4
 * 	8 7 O 5 4 3
 * 	7 6 5 4 3 2
 * 	6 O 4 3 O 1
 * 	5 4 3 2 1 V
 *
 * @param p The current position.
 * @param e The position we re looking for.
 * @param paths Matrix that will store all the computed paths to P
 * @param gm The `t_game` struct which holds the current state of the game.
 *
 * @remark `p` is freed at the end of each call to this function.
 * @remark 'V' positions are going to be replaced in the map once they move
 */
static void	path_finder(t_point *p, t_point *e, int **paths, t_game *gm)
{
	if (p->val > 0)
	{
		if ((p->r < 0 || p->r >= gm->map_h || p->c < 0 || p->c >= gm->map_w)
			|| (strchr("E1", gm->map[p->r][p->c])
				|| p->val >= paths[p->r][p->c]) || (p->r == e->r
				&& p->c == e->c))
		{
			free(p);
			return ;
		}
	}
	paths[p->r][p->c] = p->val;
	path_finder(create_point(p->r - 1, p->c, p->val + 1), e, paths, gm);
	path_finder(create_point(p->r + 1, p->c, p->val + 1), e, paths, gm);
	path_finder(create_point(p->r, p->c - 1, p->val + 1), e, paths, gm);
	path_finder(create_point(p->r, p->c + 1, p->val + 1), e, paths, gm);
	free(p);
}

/**
 * Is a companion to the function find_next_move:
 * once that finds a good candidate creates the point for it
 * and frees the old one.
 */
static t_point	*f_n_m_b(t_point **src, int r, int c, int val)
{
	t_point	*new;

	new = create_point(r, c, val);
	if (!new)
		return (NULL);
	free(*src);
	*src = new;
	return (new);
}

/**
 * @brief Given a target position (hero), and the paths using the "breadcrumbs"
 * computed by the path_finder function against a starting point in the map,
 * this function returns the next step in the shortest path toward the villain
 * against who find_hero_shortest_path was called.
 *
 * Checks the neighboring cells of the current `p` and identifies the one
 * that has the smallest step count. It then recursively calls itself with the
 * new position until it finds the target (0 is where path_finder started).
 *
 * For example, given the shortest path matrix:
 * 	P 9 8 7 6 5
 * 	9 8 7 6 O 4
 * 	8 7 O 5 4 3
 * 	7 6 5 4 3 2
 * 	6 O 4 3 O 1
 * 	5 4 3 2 1 V
 *
 * Calling this function at 'P' will return the next point with step count '9',
 * and then '8', '7', '6' and so on, until the target 'V' is reached.
 * (each possible path is equivalent, so one is picked randomly);
 *
 * @param p Pointer to a `t_point` struct that represents the current position.
 * @param paths the matrix computed by path_finder
 *
 * @return Pointer to the `t_point` that represents the next move.
 *
 * @remark The input `t_point` struct `p` is freed in each recursive call.
 * @remark each f_n_m_b call frees the old point and creates a new one. The
 * return (NULL) is to deal with a failure in such creation.
 */
t_point	*find_next_move(t_point *p, int **paths, t_game *game)
{
	t_point	*next_p;

	next_p = create_point(p->r, p->c, paths[p->r][p->c]);
	if (p->r > 0 && paths[p->r - 1][p->c] < next_p->val && !f_n_m_b(&next_p,
			p->r - 1, p->c, paths[p->r - 1][p->c]))
		return (NULL);
	if (p->r < game->map_h - 1 && paths[p->r + 1][p->c] < next_p->val
		&& !f_n_m_b(&next_p, p->r + 1, p->c, paths[p->r + 1][p->c]))
		return (NULL);
	if (p->c > 0 && paths[p->r][p->c - 1] < next_p->val && !f_n_m_b(&next_p,
			p->r, p->c - 1, paths[p->r][p->c - 1]))
		return (NULL);
	if (p->c < game->map_w - 1 && paths[p->r][p->c + 1] < next_p->val
		&& !f_n_m_b(&next_p, p->r, p->c + 1, paths[p->r][p->c + 1]))
		return (NULL);
	if (next_p->val == 0)
	{
		free(next_p);
		return (p);
	}
	//free(p); return it to the previous
	char *map = mtostr(paths,game-> map_h, game->map_w);
	if (next_p->val == INT_MAX)
		return (NULL);
	return (find_next_move(next_p, paths, game));
}

/**
 * This function computes the next best move from the position start
 * to the position end within the game map.
 * If a move is not possible (or something happens in the middle),
 * it returns NULL.
 * We make a copy of start because path_finder will free it.
 * The same is true for find_next_move
 */
t_point	*find_shortest_path(t_point *start, t_point *end, t_game *game)
{
	int		**paths;
	t_point	*start2;
	t_point	*ret;
	int		min;

	paths = init_paths_matrix(game);
	if (!paths)
		return (NULL);
	start2 = create_point(start->r, start->c, 0);
	ret = create_point(end->r, end->c, INT_MAX);
	path_finder(start2, ret, paths, game);


char *map = mtostr(paths,game-> map_h, game->map_w);

	ret = find_next_move(ret, paths, game);
	free_paths(paths);
	return (ret);
}
