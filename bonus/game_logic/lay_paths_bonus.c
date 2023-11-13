/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lay_paths_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 18:34:05 by amarabin          #+#    #+#             */
/*   Updated: 2023/11/12 19:51:07 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

/**
 * Another key function: given a target position and a map path, using
 * the "breadcrumbs" there computed by the lay_paths function against a
 * starting point in the map, returns the next step in the shortest path
 * toward that point
 *
 * Checks the neighboring cells of the current `p` and identifies the one
 * that has the smallest step count. It then recursively calls itself with the
 * new position until it finds the target (0 is where lay_paths started).
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
 * When it gets to 0 it will discard it because we got at the end the trail
 *
 * @param t     Pointer to a `t_point` struct that represents the start (or
 *              current in the recursion) position.
 * @param paths The matrix computed by lay_paths
 * @param game  The game
 * @param tb    Flag to block the trackback (is used to calculate distances)
 *              1 will trackback, 0 will not.
 *              will be returned the correct distance paths[t.r][t.c]
 * @return Pointer to the `t_point` that represents the next move.
 *
 * @remark The input `t_point` struct `t` is freed in each recursive call.
 * @remark it returns null_p() if all paths to t are blocked
 * @remark When used in the not track back mode it will return not only
 * the point with the smallest distance (p->val) but also the direction
 * of the move (0=up, 1=right, 2=down, 3=left)
 */
t_point	trak_back_paths(t_point t, int **paths, t_game *game, int tb)
{
	t_point	next_p;

	next_p = create_p(t.r, t.c, paths[t.r][t.c], t.val2);
	if (t.r > 0 && paths[t.r - 1][t.c] < next_p.val)
		next_p = create_p(t.r - 1, t.c, paths[t.r - 1][t.c], 0);
	if (t.r < game->map_h - 1 && paths[t.r + 1][t.c] < next_p.val)
		next_p = create_p(t.r + 1, t.c, paths[t.r + 1][t.c], 2);
	if (t.c > 0 && paths[t.r][t.c - 1] < next_p.val)
		next_p = create_p(t.r, t.c - 1, paths[t.r][t.c - 1], 3);
	if (t.c < game->map_w - 1 && paths[t.r][t.c + 1] < next_p.val)
		next_p = create_p(t.r, t.c + 1, paths[t.r][t.c + 1], 1);
	if (next_p.val == 0)
		return (t);
	if (next_p.val == INT_MAX)
		return (null_p());
	if (!tb)
	{
		next_p.val = paths[t.r][t.c];
		return (next_p);
	}
	return (trak_back_paths(next_p, paths, game, 1));
}

/**
 * This function is key: computes all the possible paths to a point E 'p.b' by
 * recursively exploring all the different paths sprouting of the start point
 *'p.a'. The function stops the exploration if
 * 1) it reaches a point outside the map
 * 2) if it encounters an obstacle ('P', 'V', 'D', '1' characters), depending
 * on the values contained in 'ptrn'
 * 3) if the new step count a is carrying in val is not less than the one
 * recorded by a different call to lay_paths at the current
 * path[p.a.r][p.a.c]
 * 4) or if it reaches the p.b position (end).
 * It doesn't need to returm anything because by the time it's done the path
 * matrix will be filled with the shortest paths from the start to the end
 * points (p.a to p.b).
 *
 * For example, starting from the following map:
 * 	E _ _ _ _ _
 * 	_ _ _ _ O _
 * 	_ _ O _ _ _
 * 	_ _ _ _ _ _
 * 	_ O _ _ O _
 * 	_ _ _ _ _ S
 *
 * The final content of `paths` would be this:
 * 	E 9 8 7 6 5
 * 	9 8 7 6 O 4
 * 	8 7 O 5 4 3
 * 	7 6 5 4 3 2
 * 	6 O 4 3 O 1
 * 	5 4 3 2 1 S
 *
 * @param p Contains the starting position a and the end position b.
 *           a.val should be initialized to 0
 * @param pth The matrix that will store all the computed paths to b
 * @param gm The current state of the game.
 * @param ptrn The pattern of characters in the map to be avoided.
 *        Example: for villain is E1, for hero with collectibles is EVC1,
 *        for hero without collectibles is EV1
 *
 * @remark 'V' positions will be replaced in the map once villains move
 * @remark: in order to function the attribute val of p.a has to be set to
 * INT_MAX
 */
void	lay_paths(t_pair p, int **pth, t_game *gm, char *ptrn)
{
	t_pair	pr;

	if (p.a.val > 0)
	{
		if ((p.a.r < 1 || p.a.r > gm->map_h - 2 || p.a.c < 1
				|| p.a.c > gm->map_w - 2) || strch(ptrn, gm->map[p.a.r][p.a.c])
			|| p.a.val >= pth[p.a.r][p.a.c] || (p.a.r == p.b.r
				&& p.a.c == p.b.c))
		{
			if (p.a.r == p.b.r && p.a.c == p.b.c && p.a.val < pth[p.a.r][p.a.c])
				pth[p.a.r][p.a.c] = p.a.val;
			return ;
		}
	}
	pth[p.a.r][p.a.c] = p.a.val;
	pr.a = create_p(p.a.r - 1, p.a.c, p.a.val + 1, 0);
	pr.b = p.b;
	lay_paths(pr, pth, gm, ptrn);
	pr.a = create_p(p.a.r + 1, p.a.c, p.a.val + 1, 0);
	lay_paths(pr, pth, gm, ptrn);
	pr.a = create_p(p.a.r, p.a.c - 1, p.a.val + 1, 0);
	lay_paths(pr, pth, gm, ptrn);
	pr.a = create_p(p.a.r, p.a.c + 1, p.a.val + 1, 0);
	lay_paths(pr, pth, gm, ptrn);
}
