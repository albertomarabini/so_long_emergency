/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_shortest_path_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 02:24:58 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/22 17:03:19 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

/**
 * Computes the next best move from the position start
 * to the position end within the game map.
 * If a move is not possible (or something happens in the middle),
 * it returns NULL.
 * We make a copy of start because lay_paths will free the corresponding
 * point (as well as the t_pair carrying it).
 * The same is true for trak_back_paths
 * (so ret = trak_back_paths(ret, paths, game); is ok)
 *
 * @param start The starting position
 * @param end   The ending position
 * @param game  The game struct
 * @param tb    Flag to block the trackback (is used to calculate distances)
 *              1 will trackback, 0 will not and stop at the smallest point
 * @return      The next best move
 */
t_point	find_shortest_path(t_point start, t_point end, t_game *game, int tb)
{
	int		**path;
	t_pair	pr;
	t_point	nxt;

	path = init_path_matrix(game);
	if (!path)
		return (null_p());
	pr = new_ppair(create_p(start.r, start.c, 0, 0), end);
	lay_paths(pr, path, game, "E1");
	nxt = trak_back_paths(end, path, game, tb);
	free_path(path);
	return (nxt);
}

/**
 * The Manhattan Geometry is a particular type of non euclidean geometry:
 * is a practical model for calculating distancesbin the real world when
 * movement is restricted to a grid (as in cars driving on streets).
 * The distance between 2 points instead of being
 * d = √[(x2 – x1)2 + (y2 – y1)2] is the sum of the length
 * of each segment to be traversed to get from one point to the other.
 * All is good until there are no obstacles: adding those we actually
 * need to retrace back and forth the actual path.
 * This function calculates the shortest Manhattan distance between two
 * points without any consideration on what the obstacles might be in
 * the way, beside walls.
 *
 * Inthe returned value val is the distance, r and c the vert and horiz
 * displacements.  * It also return the direction that has to be taken
 * to reach end from start (0=up, 1=right, 2=down, 3=left) in the val2 field.
 *
 *  END
 *  |\
 * r| \ val
 *  |  \
 *  |   \^ val2
 *   ---START
 *    c
 * Make sure to understand which is the actual starting point because the
 * distance is the same but the direction (given the number of obstacles in
 * the game) can be very different, not simply the opposite
 *
 * @param start The starting point.
 * @param end   The target or end point.
 * @param game  The game.
 *
 * @return      The point in one the cardinals of the start point
 *              (0=up, 1=right, 2=down, 3=left) carrying the shortest
				distance between start and end points.
 *              If path calculation fails or memory allocation fails,
 *              returns NULL.
 */
t_point	find_distance(t_point start, t_point end, t_game *game)
{
	t_point	t;

	t = find_shortest_path(start, end, game, 0);
	t.r = abs(start.r - end.r);
	t.c = abs(start.c - end.c);
	if (is_null_p(t))
		return (t);
	if (t.val == INT_MAX)
		t.val = 0;
	return (t);
}

/**
 * This function computes whether an object located at point A can reach
 * its target at point T before another moving object at point B can catch it.
 * It models a scenario where A is moving faster than B in a one-way street.
 * A must collect an item at point T while B is in pursuit. The goal is to
 * evaluate if A can reach T and reverse without colliding into B.
 * Direction is also taken into account with 0=up, 1=right, 2=down, 3=left.
 *
 * @param a - Starting point of object A
 * @param b - Starting point of object B
 * @param t - Target point T for object A
 * @param gm - Game state or map
 *
 * @return 0 if object A can safely reach T without crashing into B,
 *         1 if there is potential for a crash.
 *
 * ALIGNED CASES (all moving on the same axis, quite likely at close up)
 * case 1) B A T
 * if (at.val < bt.val && at.val2 == ba.val2)
 * if B is moving in the same direction as A it will never catch
 * case 2) A T B
 * if (ba.val >at.val && ba.val2 == bt.val2 && bt.val <
 * (at.val / 1.5))
 * if B is moving in the opposite direction as A but AB>AT it will catch up
 * only if BT < AT/1.5
 * case 3) A B T
 * if (ba.val < at.val && ba.val2 == (at.val2 + 2) % 4)
 * if B is moving in the opposite direction as A but AB<AT better run
 * UNALIGNED CASES
 * case 4)
 * T
 * |\
 * | \
 * |  \
 * B---A
 * if (bt.val < at.val / 1.5)
 * we are trying to understand if, while we get closer to the
 * target, B will be able to catch up on A while he's adapting to A's
 * change in position.
 */
int	will_it_crash(t_point a, t_point b, t_point t, t_game *gm)
{
	t_point	at;
	t_point	ba;
	t_point	bt;

	at = find_distance(a, t, gm);
	ba = find_distance(b, a, gm);
	bt = find_distance(b, t, gm);
	if (at.val < bt.val && at.val2 == ba.val2)
		return (0);
	else if (ba.val > bt.val && ba.val > at.val && ba.val2 == bt.val2
		&& bt.val < (at.val / ((VILL_TICK / HERO_TICK))))
		return (1);
	else if (ba.val < at.val && ba.val2 == (at.val2 + 2) % 4)
		return (1);
	else if (ba.val < at.val || bt.val < at.val / ((VILL_TICK / HERO_TICK)))
		return (1);
	return (0);
}
