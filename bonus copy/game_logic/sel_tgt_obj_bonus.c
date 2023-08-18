/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sel_tgt_obj_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 03:10:07 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/17 04:17:50 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

/**
 * Creates an array of potential paths to reach the targets (either corner,
 * collectible or villain) from the variable pts collecting the
 * 4 cardinals around the current point p for each path.
 */
static t_point	**potnt_tgt_moves(int ***pts, int len_pts, t_point p)
{
	t_point	**movs;
	int		i;

	movs = (t_point **)c_alloc(NULL, sizeof(t_point *) * (len_pts * 4 + 1));
	i = 0;
	while (pts[i] != NULL)
	{
		movs[i * 4] = create_pp(p.r - 1, p.c, pts[i][p.r - 1][p.c], i);
		movs[i * 4 + 1] = create_pp(p.r + 1, p.c, pts[i][p.r + 1][p.c], i);
		movs[i * 4 + 2] = create_pp(p.r, p.c - 1, pts[i][p.r][p.c - 1], i);
		movs[i * 4 + 3] = create_pp(p.r, p.c + 1, pts[i][p.r][p.c + 1], i);
		movs[++i * 4] = NULL;
	}
	return (movs);
}

/**
 * 'pts' contains a list of paths from the point p to a list of potential
 * targets (one for each matrix in paths).
 * It will collect them (to be precise will collect the length of each path
 * running out p's cardinals using the function potnt_tgt_moves), sort it by
 * the function sort, and select the first one that doesn't appear in the
 * queue 'q'.

 * NB: trak_back_paths does something similar, starting from a cardinal of
 * dest and going down to 0 to find the next step to move to.
 * Here we act in reverse: we established the target as starting point so
 * we can collect the distances directly from the cardinals around 'p'.

 * These cardinals are normally ordered from the smallest(quickest) to the
 * slowest, to the unreacheable (movs[i]->val == INT_MAX): think of when we
 * need to find the shortest way to something like a collectible.
 * But when the hero goes in "run" mode, not only we want to find an escape
 * but also the farthest point we can run to, thus this is why we need a
 * different sorting
 *
 * Best is a flag to indicate whether to select the best target anyway,
 * even if was previously in 'q'' and a new target wasn't found.
 *
 * @param pts     List of paths from p toward potential targets.
 * @param p       Current point of reference (starting point)
 * @param q       Queue with the last targets examined.
 * @param m.sort  Function pointer for sorting the paths.
 * @param m.best  Flag to indicate whether to select the best available target
 *                anyway even if no new actual target is found.
 */
void	sel_tgt(int ***pts, t_point p, int (*q)[TGT_Q_LEN],
		t_sel_tgt_obj_mechanics m)
{
	t_point	**movs;
	int		i;
	int		len_pts;

	len_pts = 0;
	while (pts[len_pts] != NULL)
		len_pts++;
	i = 4 * len_pts;
	movs = potnt_tgt_moves(pts, len_pts, p);
	m.sort(movs, i);
	i = 0;
	while (movs[i] != NULL)
	{
		if (movs[i]->val != INT_MAX && !a_contains(*q, movs[i]->val2,
				TGT_Q_LEN))
			break ;
		i++;
	}
	if (movs[i] != NULL)
		a_shift(*q, movs[i]->val2, TGT_Q_LEN);
	else
		null_a(*q, TGT_Q_LEN);
	if (m.best && movs[i] == NULL)
		a_shift(*q, movs[0]->val2, TGT_Q_LEN);
	free_ap(movs);
}

/**
 * Uses Manhattan geometry principles to find if the closest
 * approaching villains will be able to reach the hero before it reaches the
 * target (whatever the target might be).
 * @param   t           The target point.
 * @param   gm          The game.
 * @param   c_points    Array of collectible coordinates (or exits when
 *                      the collectibles are exausted)
 * @param   v_paths_m   The paths toward the hero of all the villains in
 *                      the field from where we select the closest only.
 * @param   pattern_c   String used to lay down a valid path toward a
 *                      collectible as per the lay_paths function
 *
 * @return      A pointer to a t_point which represents the best path for the
 *              hero from the it's current position to reach the target.
 *              The 'val' field of this t_point
 *              contains the Manhattan crash result test with the closest
 *              approaching villain.
 *
 * Internal workings:
 * The function first initializes a paths matrix,  then creates a start
 * point for the hero and fills the path using lay_paths.
 * It then select as target object the closest villains and computes the
 * potential collision with the hero
 */
t_point	is_path_to_tgt_nsafe(t_point t, t_game *gm, int ***v_paths_m,
		char *pattern_c)
{
	int		**curr_tgt_path;
	t_point	strt;
	t_point	nxt;
	int		tgt_v[TGT_Q_LEN];
	int		i;

	curr_tgt_path = init_path_matrix(gm);
	if (!curr_tgt_path)
		return (null_p());
	strt = create_p(gm->hero_r, gm->hero_c, 0, 0);
	lay_paths(new_ppair(strt, t), curr_tgt_path, gm, pattern_c);
	nxt = trak_back_paths(t, curr_tgt_path, gm, 1);
	if (!is_null_p(nxt))
	{
		i = 0;
		null_a(tgt_v, TGT_Q_LEN);
		while (trn(gm->vills > TGT_Q_LEN + 1, TGT_Q_LEN + 1, gm->vills) > i)
		{
			sel_tgt(v_paths_m, t, &tgt_v, sto_mech(sort_ap, 1));
			nxt.val = will_it_crash(strt, *(gm->a_vills[tgt_v[0]]), t, gm);
			i = trn(nxt.val != 0, TGT_Q_LEN + 1, i + 1);
		}
	}
	free_path(curr_tgt_path);
	return (nxt);
}
