/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_hero_move_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 02:04:30 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/18 01:57:10 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

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
		int ***v_paths_m, char *pattern_c)
{
	int		i;
	int		r;
	int		c;
	t_point	end;

	i = 0;
	while (CORNER_COUNT + 1 > i++)
	{
		sel_tgt(sf_gm->c_paths_m, hero, &(sf_gm->tgt_el_q),
			sto_mech(rev_sort_ap, 0));
		if (is_null_a(sf_gm->tgt_el_q, TGT_Q_LEN) || (within(hero.r,
					sf_gm->c_points[sf_gm->tgt_el_q[0]]->r, 2) && within(hero.c,
					sf_gm->c_points[sf_gm->tgt_el_q[0]]->c, 2)))
			continue ;
		r = sf_gm->c_points[sf_gm->tgt_el_q[0]]->r;
		c = sf_gm->c_points[sf_gm->tgt_el_q[0]]->c;
		end = create_p(r, c, 0, 0);
		hero = is_path_to_tgt_nsafe(end, sf_gm, v_paths_m, pattern_c);
		if (!is_null_p(hero) && !hero.val)
			return (hero);
	}
	return (hero);
}

/**
 * There are two states for the hero: the first is the normal state, when it
 * tries to collect items, avoid villains and go for an exit by following the
 * shortest path.
 * The second one is the run state, where no safe targets are on sight and we
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
 * @param v_paths_m Paths of the villains toward the hero.
 * @param pattern_c The pattern used for scanning the map in search for a
 *                  target (in this case the corner).
 * @return Returns the hero's point after the function's operations.
 */
static t_point	handle_game_run_state(t_game *gm, int ***v_paths_m,
		char *pattern_c)
{
	t_game	*sf_gm;
	t_point	hero;
	t_point	run;

	sf_gm = create_safe_game(gm, 2);
	// IF NULL
	sf_gm->c_paths_m = collect_corn_paths(sf_gm, &(sf_gm->c_points), pattern_c);
	hero = create_p(gm->hero_r, gm->hero_c, 0, 0);
	gm->run_unsafe = 1;
	hero = select_run_state_escape_corner(sf_gm, hero, v_paths_m, pattern_c);
	if (!is_null_p(hero) && !hero.val)
		gm->run_unsafe = 0;
	if (!is_null_a(sf_gm->tgt_el_q, TGT_Q_LEN))
	{
		gm->run_to = *(sf_gm->c_points[sf_gm->tgt_el_q[0]]);
		run = find_distance(hero, gm->run_to, gm);
		gm->run = trn(run.r > run.c, run.r, run.c) / 3 * 2;
	}
	null_a(gm->tgt_el_q, TGT_Q_LEN);
	free_tgt_path_structs(sf_gm->c_paths_m, sf_gm->c_points);
	free_safe_map(sf_gm);
	return (hero);
}

/**
 * @brief Checks if the game is in running state and determines the next
 *        action for the hero.
 *
 * If the game is in run mode, if the run was in unsafe mode from the
 * beginning we check if this situation has changed and, in case, exit
 * the run mode. If keep being unsafe we compute the next move.
 *
 * @param gm Pointer to the current game state.
 * @param v_paths_m The paths of the villains.
 *
 * @return Returns the next position for the hero if we keep the running mode
 *         If the game is not in a running state a null_p() is returned.
 */
static t_point	is_game_in_run_state(t_game *gm)
{
	t_game	*sf_gm;
	t_point	hero;
	int		n_sf;

	hero = create_p(gm->hero_r, gm->hero_c, 0, 0);
	if (gm->run)
	{
		printf("Running, move:%i\n", gm->run);
		n_sf = is_path_to_tgt_nsafe(gm->run_to, gm, gm->v_paths_m, "EV1").val;
		if (gm->run_unsafe == 0 && n_sf == 1)
			gm->run = 0;
		else
		{
			gm->run--;
			sf_gm = create_safe_game(gm, 2);
			hero = find_shortest_path(hero, gm->run_to, sf_gm, 1);
			free_safe_map(sf_gm);
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
 *        and the second the corresponding path to the hero. Using
 *        sel_tgt, we pick the next target to consider by placing it
 *        at the beginning of the queue. 'targets' is set to at least
 *        TGT_Q_LEN + 1 so we come out from the function with an unsafe path
 *        and we trigger the run mode.
 *      - hero = is_path_to_tgt_nsafe(...): Determines if there's a safe
 *        path toward the new target selected and updates the hero's
 *        next step accordingly. If no safe path is found, the loop
 *        continues to check other paths.
 *
 * If after going through the target queue for a specified number of tgts,
 * no safe path is found, the game switches to run mode.
 *
 * @return A t_point structure representing the next step for the hero.
 */
static t_point	handle_game_normal_state(t_game *gm, int tgts, char *ptrn_c)
{
	int		i;
	t_point	end;
	t_point	hero;

	i = 0;
	hero = create_p(gm->hero_r, gm->hero_c, 0, 0);
	while (tgts > i)
	{
		if (is_null_a(gm->tgt_el_q, TGT_Q_LEN))
			sel_tgt(gm->c_paths_m, hero, &(gm->tgt_el_q), sto_mech(sort_ap, 1));
		end = create_p(gm->c_points[gm->tgt_el_q[0]]->r,
			gm->c_points[gm->tgt_el_q[0]]->c, 0, 0);
		hero = is_path_to_tgt_nsafe(end, gm, gm->v_paths_m, ptrn_c);
		if (!is_null_p(hero) && !hero.val)
			break ;
		if (i == tgts - 1)
		{
			hero = handle_game_run_state(gm, gm->v_paths_m, ptrn_c);
			null_a(gm->tgt_el_q, TGT_Q_LEN);
			break ;
		}
		i++;
	}
	return (hero);
}

/**
 * We need to find the best move for the hero.
 * To do it we'll find all the possible next possible best move of the
 * hero (toward a collectible first and the exit later) and the best
 * moves of the villains toward the hero.
 * so we produce all those mappings (all the vills, all the colls not
 * taken yet, the exits) with lay_paths and we sort
 * them by the shortest path in the four cardinals (up, down, left, right)
 * around the hero using sel_tgt. Then with find_best_hero_move we
 * select the next best move.
 */
t_point	find_hero_move(t_game *gm)
{
	char	*pattern_c;
	t_point	hero;
	int		targets;

	gm->v_paths_m = collect_vill_paths(gm);
	hero = is_game_in_run_state(gm);
	if (!is_null_p(hero))
	{
		free_paths_matrix(gm->v_paths_m);
		return (hero);
	}
	targets = collect_tgt_paths(gm, &pattern_c);
	if (targets > TGT_Q_LEN + 1)
		targets = TGT_Q_LEN + 1;
	hero = handle_game_normal_state(gm, targets, pattern_c);
	free_paths_matrix(gm->v_paths_m);
	free_tgt_path_structs(gm->c_paths_m, gm->c_points);
	return (hero);
}
