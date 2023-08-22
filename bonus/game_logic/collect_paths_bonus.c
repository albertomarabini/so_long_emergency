/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_paths_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 02:04:30 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/22 17:06:59 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

/**
 * Calculates the paths for each corner of the game.
 * @param n         The number of a_corners we want to use
 * @param game      The game.
 * @param c_points  Pointer to the array of t_point which will store the
 *                  a_corners.
 * @return      it sets  gm->c_points[i] and gm->c_paths_m used for target
 *              exploration using each map's corner position and relative
 *              paths from each to the hero.
 */
void	collect_corn_paths(t_game *gm, char *pattern_c)
{
	int		i;
	t_point	stt;
	t_point	*c;

	i = 0;
	if (!inst_tgt_path_structs(CORNER_COUNT, &(gm->c_paths_m), &(gm->c_points)))
		c_throw(gm, NULL, NULL);
	while (i < CORNER_COUNT)
	{
		c = gm->a_corners[i++];
		gm->c_points[i - 1] = p_to_pp(*c);
		gm->c_paths_m[i - 1] = init_path_matrix(gm);
		stt = create_p(gm->a_corners[i - 1]->r, gm->a_corners[i - 1]->c, 0, 0);
		lay_paths(new_ppair(stt, hero_p(gm)), gm->c_paths_m[i - 1], gm,
			pattern_c);
	}
	gm->c_paths_m[i] = NULL;
	gm->c_points[i] = NULL;
}

/**
 * Calculates the paths for each exit of the game.
 *
 * @param game      The game.
 * @param c_points  Pointer to the array of t_point which will store the exits.
 * @param pattern_c The string that defines which characters are acceptable
 *                  for parsing the map.
 * @return      A 3D array of integer representing the paths from each exit
 *              to the hero. The last element of the outermost array is set
 *              to NULL as a sentinel.
 *              If memory allocation fails the function returns NULL.
 */
static int	***collect_exit_paths(t_game *gm, t_point ***c_points,
		char *pattern_c, t_point hero)
{
	int		i;
	t_point	stt;
	int		***c_paths_m;

	i = 0;
	if (!inst_tgt_path_structs(len_ap(gm->a_exits), &c_paths_m, c_points))
		c_throw(gm, NULL, NULL);
	while (gm->a_exits[i])
	{
		if (gm->is_safe && gm->map[gm->a_colls[i]->r][gm->a_colls[i]->c] == 1)
			continue ;
		(*c_points)[i] = p_to_pp(*(gm->a_exits[i]));
		c_paths_m[i++] = init_path_matrix(gm);
		if (!*c_paths_m[i - 1])
			return (NULL);
		stt = create_p(gm->a_exits[i - 1]->r, gm->a_exits[i - 1]->c, 0, 0);
		lay_paths(new_ppair(stt, hero), c_paths_m[i - 1], gm, pattern_c);
	}
	c_paths_m[i] = NULL;
	(*c_points)[i] = NULL;
	return (c_paths_m);
}

/**
 * Calculates the paths for each collectible not yet collected
 * (gm->a_colls[j]->val == 0) in the game to reach the hero.
 * is cunter intuitive but are the paths for the hero to reach the
 * collectibles in reverse.
 *
 * @param game      The game.
 * @param c_points  Pointer to the array of t_point which will store the colls.
 * @param pattern_c The string that defines which characters are acceptable
 *                  for parsing the map.
 * @return      A 3D array of integer representing the paths from each coll
 *              to the hero.
 */
static int	***collect_coll_paths(t_game *gm, t_point ***c_points,
		char *pattern_c, t_point hero)
{
	int		i;
	int		j;
	t_point	stt;
	int		***c_paths_m;

	i = 0;
	j = 0;
	if (!inst_tgt_path_structs(len_ap(gm->a_colls), &c_paths_m, c_points))
		c_throw(gm, NULL, NULL);
	while (gm->a_colls[j++])
	{
		if (gm->a_colls[j - 1]->val != 0 || (gm->is_safe
				&& gm->map[gm->a_colls[j - 1]->r][gm->a_colls[j - 1]->c] == 1))
			continue ;
		(*c_points)[i] = p_to_pp(*(gm->a_colls[j - 1]));
		c_paths_m[i++] = init_path_matrix(gm);
		if (!c_paths_m[i - 1])
			return (NULL);
		stt = create_p(gm->a_colls[j - 1]->r, gm->a_colls[j - 1]->c, 0, 0);
		lay_paths(new_ppair(stt, hero), c_paths_m[i - 1], gm, pattern_c);
	}
	c_paths_m[i] = NULL;
	(*c_points)[i] = NULL;
	return (c_paths_m);
}

/**
 * Collects targets information depending on what the hero is going after
 * (either collectibles or exits) and collects the related informations.
 *
 * @param gm         The game.
 * @param ptrn_c     The pattern string used to scan the map depending
 *                   on what objects we are going after.
 *
 * @return Returns the number of current targets depending on the game stage.
 *         are to be considered return objects also game->c_points,
 *         game->c_paths_m, ptrn_c
 */
int	collect_tgt_paths(t_game *gm, char **ptrn_c)
{
	int		targets;
	t_point	hero;

	hero = create_p(gm->hero_r, gm->hero_c, INT_MAX, 0);
	if (gm->colls > 0)
	{
		*ptrn_c = "EV1";
		targets = gm->colls;
		gm->c_paths_m = collect_coll_paths(gm, &(gm->c_points), *ptrn_c, hero);
	}
	else
	{
		*ptrn_c = "V1";
		targets = gm->exits;
		gm->c_paths_m = collect_exit_paths(gm, &(gm->c_points), *ptrn_c, hero);
	}
	return (targets);
}

/**
 * Calculates the paths for each villain in the game to reach the hero.
 *
 * @param       game    The game.
 *
 * @return      it sets gm->v_paths_m representing the paths from each vill
 *              to the hero.
 */
void	collect_vill_paths(t_game *gm)
{
	int		i;
	t_point	stt;
	t_point	hero;

	i = 0;
	hero = create_p(gm->hero_r, gm->hero_c, INT_MAX, 0);
	gm->v_paths_m = (int ***)c_alloc(gm, sizeof(int **) * (gm->vills + 1));
	while (i < gm->vills)
	{
		gm->v_paths_m[i++] = init_path_matrix(gm);
		stt = create_p(gm->a_vills[i - 1]->r, gm->a_vills[i - 1]->c, 0, 0);
		lay_paths(new_ppair(stt, hero), gm->v_paths_m[i - 1], gm, "E1");
	}
	gm->v_paths_m[i] = NULL;
}
