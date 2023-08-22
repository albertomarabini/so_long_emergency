/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_hero_move_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 02:04:30 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/22 08:35:25 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

void	print_target_points(t_point **movs)
{
	int	i;

	i = 0;
	while (movs[i])
	{
		printf("Tgt Point %d: r=%d, c=%d\n", i, movs[i]->r, movs[i]->c);
		i++;
	}
}

static t_point	handle_game_normal_state(t_game *gm, int tgts, char *ptrn_c,
					t_point *end);

/**
 * Selects the farthest safe corner to escape to during the game's run state.
 *
 * If no safe corner is found, it attempts to pick the best target available
 * from the sf_gm->tgt_el_q queue.This is ensured by iterating through the
 * queue CORNER_COUNT + 1 times (it should turn the queue around and bring
 * back the first item at the beginning of the queue).
 * Together with safety the selected corner is evaluated to ensure the hero
 * is not too close to it.
 *
 * @param sf_gm The game
 * @param hero The current position of the hero.
 * @param v_paths_m Paths of the villains toward the hero.
 * @param pattern_c The pattern used for scanning the map in search for a
 *                  target (in this case the corner).
 * @return Returns the optimal corner point for the hero to escape to.
 */
static t_point	select_run_state_escape_corner(t_game *sf_gm, t_point hero,
		char *pattern_c, t_point *end)
{
	int		i;
	int		r;
	int		c;
	t_point	nxt;

	i = 0;
	null_a(sf_gm->tgt_el_q, TGT_Q_LEN);
	while (CORNER_COUNT + 1 > i++)
	{
		sel_tgt(sf_gm->c_paths_m, hero, &(sf_gm->tgt_el_q),
			sto_mech(rev_sort_ap, 0));
		if (is_null_a(sf_gm->tgt_el_q, TGT_Q_LEN) || (within(hero.r,
					sf_gm->c_points[sf_gm->tgt_el_q[0]]->r, 2) && within(hero.c,
					sf_gm->c_points[sf_gm->tgt_el_q[0]]->c, 2)))
			continue ;
		*end = pp_to_p(sf_gm->c_points[sf_gm->tgt_el_q[0]]);
		nxt = is_path_to_tgt_nsafe(*end, sf_gm, sf_gm->v_paths_m, pattern_c);
		if ((!is_null_p(nxt) && !nxt.val) || i - 1 == CORNER_COUNT)
			return (nxt);
	}
	return (hero);
}

/**
 * There are two games states: the normal state and the safe state, where no
 * safe targets are on sight and we
 * just need to run away from the villains as far as we can, often avoiding
 * the villains when they are too close.
 * to do this in create_safe_game we create a map where the sorroundings of
 * each villain is filled with 1s, as are impenetrable areas, avoiding
 * the temptation of the hero to sneak around them
 * Being an attempt for life, it works in 2 ways: safe and unsafe mode. In
 * safe mode we just find the farthest safe corner we can reach and we go there
 * by saving the point in gm->run_to and establishing inside gm->run a
 * reasonable number of steps to take in order to consider ourselves free
 * from the cloth (we take the bigger Manhattan displacement of the hero
 * the destinationfr point): we also need to lose the villains.
 *     H
 *     |\
 * run | \
 *     |  \
 *     |---P
 * Unsafe mode kicks in when we don't find a safe corner to run to and we just
 * run to the farthest corner possible (the first value currently in the
 * sf_gm->tgt_el_q) hoping for the best. Within is_game_in_run_state we will
 * check if the safety has changed and in case we will re-evaluate the
 * destination.
 * @param gm The game
 * @param pattern_c The pattern used for scanning the map in search for a
 *                  target (in this case the corner).
 * @return Returns the hero's point after the function's operations.
 */
static t_point	handle_game_safe_state(t_game *gm, char *pattern_c)
{
	t_game	*sf_gm;
	t_point	hero;
	t_point	run;
	t_point	nxt;
	int		tgt_count;

	sf_gm = create_safe_game(gm, 3);
	sf_gm->v_paths_m = gm->v_paths_m;
	tgt_count = collect_tgt_paths(sf_gm, &pattern_c);
	print_target_points(sf_gm->c_points);
	if (tgt_count > TGT_Q_LEN + 1) // tgt_count == 0???
		tgt_count = TGT_Q_LEN + 1;
	nxt = handle_game_normal_state(sf_gm, tgt_count, pattern_c, &gm->run_to);
	if (is_null_p(nxt) || nxt.val)
	{
		printf("safe mode:looking for a corner\n");
		free_tgt_path_structs(sf_gm->c_paths_m, sf_gm->c_points);
		collect_corn_paths(sf_gm, pattern_c);
		hero = create_p(gm->hero_r, gm->hero_c, 0, 0);
		nxt = select_run_state_escape_corner(sf_gm, hero, pattern_c,
			&gm->run_to);
	}
	if (!is_null_a(sf_gm->tgt_el_q, TGT_Q_LEN))
	{
		run = find_distance(nxt, gm->run_to, gm);
		gm->run = trn(run.r > run.c, run.r, run.c);
	}
	null_a(gm->tgt_el_q, TGT_Q_LEN);
	// printf("gm->run_to (%i,%i)\n", gm->run_to->r, gm->run_to->c);
	free_tgt_path_structs(sf_gm->c_paths_m, sf_gm->c_points);
	free_safe_game(sf_gm);
	return (nxt);
}

/**
 * @brief Checks if the game is in running state and determines the next
 *        action for the hero.
 *
 * If the game is in run mode, if the run was unsafe mode from the
 * beginning we check if this situation has changed and, in case, exit
 * the run mode. If keep being unsafe we compute the next move.
 *
 * @param gm Pointer to the current game state.
 *
 * @return Returns the next position for the hero if we keep the running mode
 *         If the game is not in a running state a null_p() is returned.
 */
static t_point	is_game_in_run_state(t_game *gm)
{
	t_game	*sf_gm;
	t_point	hero;

	hero = create_p(gm->hero_r, gm->hero_c, 0, 0);
	if (gm->run)
	{
		printf("run %i\n", gm->run);
		if (is_path_to_tgt_nsafe(gm->run_to, gm, gm->v_paths_m, "EV1").val)
			gm->run = 0;
		else
		{
			gm->run--;
			sf_gm = create_safe_game(gm, 2);
			hero = find_shortest_path(hero, gm->run_to, sf_gm, 1);
			free_safe_game(sf_gm);
			return (hero);
		}
	}
	return (null_p());
}

/**
	* Handles the logic for the hero's movement within the game normal state.
	*
	* This function determines the next move for the hero based on the
	* current game state and the surrounding targets and villains.
	*
	* @param gm          The game state object containing information about
	*                    the current position of the hero, the targets,
	*                    and the paths.
	* @param tgts      The maximum number of tgts to determine the hero's
	*                    next move.
	* @param pattern_c   The pattern to parse the map by according to the
	*                    coll type.
	*
	* @details
	*      - gm->tgt_el_q: A queue of indexes of the arrays gm->c_points
	*        and gm->c_paths_m. The first represent the t_point of each target
	*        and the second the corresponding path to it. Using
	*        sel_tgt, we pick the next target to consider by placing it
	*        at the beginning of the queue (the queue is to keep track of those
	*        who have been previously examined). 'targets' is set to at least
	*        TGT_Q_LEN + 1 so even if there are no safe paths,
		we come out from
	*        the function with an unsafe path and we trigger the run mode.
	*      - hero = is_path_to_tgt_nsafe(...): Determines if there's a safe
	*        path toward the new target selected and updates the hero's
	*        next step accordingly. If no safe path is found, the loop
	*        continues to check other paths.
	* so the following code means
	* //if the queue is empty, select a target and place it into the queue
	* if (is_null_a(gm->tgt_el_q,...))
	*	sel_tgt(gm->c_paths_m, hero, &(gm->tgt_el_q)...);
	* // then create a point variable with the data of this target
	* 	end = create_p(gm->c_points[gm->tgt_el_q[0]]->r...);
	*	// and see if the path to it is safe
	* hero = is_path_to_tgt_nsafe(end, gm, gm->v_paths_m, ptrn_c);
	* If after going through the target queue for a specified number of tgts,
	* no safe path is found, the game switches to run mode.
	*
	* @return A t_point structure representing the next step for the hero.
	*/
static t_point	handle_game_normal_state(t_game *gm, int tgts, char *ptrn_c,
		t_point *end)
{
	int						i;
	t_point					hero;
	t_point					nxt;
	t_sel_tgt_obj_mechanics	mech;

	if (gm->is_safe)
		// mech = sto_mech(rev_sort_ap, 0);
		mech = sto_mech(sort_avg_ap, 0);
	else
		mech = sto_mech(sort_ap, 0);
	i = 0;
	hero = create_p(gm->hero_r, gm->hero_c, 0, 0);
	while (tgts > i)
	{
		if (is_null_a(gm->tgt_el_q, TGT_Q_LEN))
			sel_tgt(gm->c_paths_m, hero, &(gm->tgt_el_q), mech);
		printf("Queue at %i: %i,%i,%i\n", i, gm->tgt_el_q[0], gm->tgt_el_q[1],
			gm->tgt_el_q[2]);
		if (is_null_a(gm->tgt_el_q, TGT_Q_LEN)) // no tgt found
			return (null_p());
		*end = pp_to_p(gm->c_points[gm->tgt_el_q[0]]);
		nxt = is_path_to_tgt_nsafe(*end, gm, gm->v_paths_m, ptrn_c);
		if (!is_null_p(nxt) && !nxt.val)
			break ;
		if (i == tgts - 1 && !gm->is_safe)
		{
			printf("going to safe mode\n");
			nxt = handle_game_safe_state(gm, ptrn_c);
			null_a(gm->tgt_el_q, TGT_Q_LEN);
			break ;
		}
		sel_tgt(gm->c_paths_m, hero, &(gm->tgt_el_q), mech);
		i++;
	}
	// printf("nxt (%i,%i)\n", nxt.r, nxt.c);
	return (nxt);
}

/**
 * We need to find the best move for the hero.
 * To do it first we'll find all the possible next bests moves of the
 * hero (toward a collectible first and the exit later)
 * (collect_tgt_paths(gm, &pattern_c) that will populate gm->c_paths_m
 * and  gm->c_points) and of the
 * villains toward the hero (collect_vill_paths(gm) that will populate
 * gm->v_paths_m).
 * The goal is to determine the closest targets and the closest treaths
 * and by squaring each other with the function will_it_crash(h, t, v)
 * understand if taking a certain move is safe.
 * so we produce all those mappings (all the vills, all the colls not
 * taken yet, the exits) and we sort them by the shortest path in the
 * four cardinals (up, down, left, right) using lay_paths/trak_back_paths
 * around the hero using sel_tgt.
 * obviously at times no moves to the closest target i going to be safe
 * (obviously we have been followed), so then we switch to safe mode.
 * In safe mode we have previously established what is the best target
 * and we are "making a run" to it.
 * nxt will contain the hero next move;
 */
t_point	find_hero_move(t_game *gm)
{
	char	*pattern_c;
	t_point	nxt;
	t_point	end;
	int		tgt_count;

	collect_vill_paths(gm);
	nxt = is_game_in_run_state(gm);
	if (!is_null_p(nxt))
	{
		free_paths_matrix(gm->v_paths_m);
		return (nxt);
	}
	tgt_count = collect_tgt_paths(gm, &pattern_c);
	if (tgt_count > TGT_Q_LEN + 1)
		tgt_count = TGT_Q_LEN + 1;
	nxt = handle_game_normal_state(gm, tgt_count, pattern_c, &end);
	if (is_null_p(nxt))
		nxt = create_p(gm->hero_r, gm->hero_c, 0, 0);
	free_paths_matrix(gm->v_paths_m);
	free_tgt_path_structs(gm->c_paths_m, gm->c_points);
	return (nxt);
}
