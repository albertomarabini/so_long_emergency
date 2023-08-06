/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_path_finder_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 10:47:19 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/06 13:05:46 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

/* DEBUG */
char	*mtostr(int **matrix, int rows, int cols)
{
	int		length;
	char	*str;

	// Calculate the length of the string.
	// Assuming that an integer takes up to 10 characters,
	// plus 2 for the space and the newline, plus 1 for the null terminator.
	length = rows * (cols * (10 + 2) + 1) + 1;
	str = c_alloc(length);
	if (!str)
	{
		// handle allocation failure
		return (NULL);
	}
	str[0] = '\0'; // Start with an empty string
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			char buf[12]; // Buffer to hold one integer
			if (matrix[i][j] == INT_MAX)
				sprintf(buf, "%s ", "IM");
			else
				sprintf(buf, "%2d ", matrix[i][j]);
			strcat(str, buf); // Append to the string
		}
		strcat(str, "\n"); // Newline after each row
	}
	return (str);
}

void	free_paths(int **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		c_frr("", paths[i++]);
	c_frr("", paths);
}
void	free_paths_matrix(int ***paths)
{
	int	i;

	i = 0;
	while (paths[i])
		free_paths(paths[i++]);
	c_frr("", paths);
}

/**
 * creates a new row for paths initialized at INT_MAX
 */
static int	*init_paths_row(t_game *game, int r)
{
	int	*row;
	int	i;

	row = (int *)c_alloc(sizeof(int) * (game->map_w));
	if (!row)
		return (NULL);
	i = 0;
	while (i < game->map_w)
		row[i++] = INT_MAX;
	return (row);
}

/**
 * initializes a new matrix of paths
 */
static int	**init_paths_matrix(t_game *game)
{
	int	**paths;
	int	r;

	paths = (int **)c_alloc(sizeof(int *) * (game->map_h + 1));
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

typedef struct s_point_pair
{
	t_point		a;
	t_point		b;
}				t_point_pair;

// Function to instantiate a t_point_pair
t_point_pair	create_pp(t_point a, t_point b)
{
	t_point_pair	pair;

	pair.a = a;
	pair.b = b;
	return (pair);
}

void	print_point(t_point *p, const char *label)
{
	if (p != NULL)
	{
		printf("%s -> r: %d, c: %d, val: %d, val2: %d\n", label, p->r, p->c,
			p->val, p->val2);
	}
	else
	{
		printf("%s is NULL\n", label);
	}
}

void	print_points(t_point **points, char *label)
{
	int	i;

	i = 0;
	printf("%s[\n", label);
	while (points[i] != NULL)
	{
		if (points[i] != NULL)
		{
			printf("\t{ r: %d, c: %d, val: %d, val2: %d },\n", points[i]->r,
				points[i]->c, points[i]->val, points[i]->val2);
		}
		else
		{
			printf("\tNULL,\n");
		}
		i++;
	}
	printf("]\n");
}

/**
 *we are using a inappropriately using a point as a collection of values
 */
int	p_contains(t_point *p, int val)
{
	return (p->r == val || p->c == val || p->val == val);
}
void	p_shift(t_point *p, int val)
{
	p->val = p->c;
	p->c = p->r;
	p->r = val;
}

int	abs(int val)
{
	if (val < 0 && val > INT_MIN)
		return (-val);
	return (val);
}

int	len_o(void **o)
{
	int	i;

	i = 0;
	while (o[i] != NULL)
		i++;
	return (i);
}

int	len_p(t_point **p)
{
	int	i;

	i = 0;
	while (p[i] != NULL)
		i++;
	return (i);
}

/**
 * This function calculates the distance between two points in absolute terms
 * using Manhattan Geometry. The Manhattan Geometry is a particular type of
 * non euclidean geometry is a practical model for calculating distances
 * are calculated in the real world when movement is restricted to a grid
 * (as in cars driving on streets). Thus here the distance between 2 points
 * instead of being d = √[(x2 – x1)2 + (y2 – y1)2] is the sum of the length
 * of each segment to be traversed to get from one point to the other.
 *
 * @param[in]   p1   Pointer to the first point (of type t_point).
 * @param[in]   p2   Pointer to the second point (of type t_point).
 *
 * @return      The Manhattan distance between points p1 and p2.
 *
 * @note        The order of the input points does not affect the result.
 */
int	manhtn_dst(t_point *p1, t_point *p2)
{
	return (abs(p1->r - p2->r) + abs(p1->c - p2->c));
}

/**
 * Given a, b, c, it will find out if a point  A will be able to reach a point C
 * before another moving object B will be able to.  These are scenarios where A
 * will be able to reach C safely before to get caught by C
 * Think of a one way street where we have a car A moving faster than the

	* car B. Both must collect something at point C (target). I need to calculate if
 * A will be able to reach C and turn back before to crash on B
 * B          A
 *  A     AND  C
 *   C          B
 * if AB < AC
	&& BC < AC/1.5 (we factor in that ghosts are slower) we decide there
 * will be a crash.
 */
int	manhtn_crash(t_point *a, t_point *b, t_point *c)
{
	t_point	*tmp;
	int		ret;

	tmp = create_p(manhtn_dst(a, b), manhtn_dst(a, c), manhtn_dst(b, c));
	ret = tmp->r < tmp->c && tmp->val < tmp->c / 1.5;
	c_frr("", tmp);
	return (ret);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char		*d;
	const char	*s;
	size_t		i;

	if (!dest || !src)
		return (dest);
	i = 0;
	d = (char *)dest;
	s = (const char *)src;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}

/**
 * Concatenates 2 t_point** arrays.
 *
 * @return      A pointer to the concatenated points array.
 *              NULL for memory allocation failure.
 */
t_point	**concat_p(t_point **a1, t_point **a2)
{
	int		l1;
	int		l2;
	int		i;
	t_point	**ac;

	l1 = 0;
	l2 = 0;
	l1 = len_p(a1);
	l2 = len_p(a2);
	ac = (t_point **)c_alloc(sizeof(t_point *) * (l1 + l2 + 1));
	if (!ac)
		return (NULL);
	ft_memcpy(ac, a1, l1 * sizeof(int **));
	ft_memcpy(ac + l1, a2, l2 * sizeof(int **));
	ac[l1 + l2] = NULL;
	c_frr("", a1);
	c_frr("", a2);
	return (ac);
}

/**
 * Concatenates int *** matrixs.
 *
 * @return      A pointer to the concatenated matrix.
 *              NULL for memory allocation failure.
 */
int	***concat_paths(int ***p1, int ***p2)
{
	int	l1;
	int	l2;
	int	***pc;
	int	i;

	l1 = 0;
	l2 = 0;
	l1 = len_o((void **)p1);
	l2 = len_o((void **)p2);
	pc = (int ***)c_alloc(sizeof(int **) * (l1 + l2 + 1));
	ft_memcpy(pc, p1, l1 * sizeof(int **));
	ft_memcpy(pc + l1, p2, l2 * sizeof(int **));
	pc[l1 + l2] = NULL;
	c_frr("", p1);
	c_frr("", p2);
	return (pc);
}

/**
 * @brief This function computes all the possible paths to a point E 'pp->b' by
 * recursively exploring all the different paths sprouting of the start point
 *'pp->a'. The function stops the exploration if
 * 1)it reaches a point outside the map,
 * 2) if it encounters an obstacle ('P', 'V', 'D', '1' characters), depending
 * on the value of 'ptrn',
 * 3) or if the new step count `stp` is not less than the one recorded
 * by a different call to lay_paths
 * 4) or if it reaches the E position.
 * It doesn't need to returm anything because by the time it's done the paths
 * matrix will be filled with the shortest path from S to E (pp->a to pp->b).
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
 * @param pp Contains the starting position a and the end position b.
 * @param paths Matrix that will store all the computed paths to b
 * @param gm The current state of the game.
 * @param ptrn The pattern of characters in the map to be avoided.
 *        Example: for villain is E1, for hero with collectibles is EVC1,
 *        for hero without collectibles is EV1
 *
 * @remark `pp` and 'pp->a' are freed at the end of each call to this function.
 * @remark 'V' positions are going to be replaced in the map once villains move
 */
static void	lay_paths(t_point_pair pp, int **paths, t_game *gm, char *ptrn)
{
	t_point_pair	pr;

	if (pp.a.val > 0)
	{
		// print_point(pp->a,"a");
		// print_point(pp->b,"b");
		// char c= gm->map[pp->a->r][pp->a->c];
		// printf("paths value=%d, map value=%c\n", paths[pp->a->r][pp->a->c], c);
		if ((pp.a.r < 0 || pp.a.r >= gm->map_h || pp.a.c < 0 || pp.a.c >= gm->map_w) ||
		strchr(ptrn,gm->map[pp.a.r][pp.a.c]) || pp.a.val >= paths[pp.a.r][pp.a.c]
			|| (pp.a.r == pp.b.r && pp.a.c == pp.b.c))
		{
			return ;
		}
	}
	paths[pp.a.r][pp.a.c] = pp.a.val;
	// printf("lay_paths:\n");
	// printf("%s", mtostr(paths, 5, 6));
	pr.a = create_sp(pp.a.r - 1, pp.a.c, pp.a.val + 1);
	pr.b = pp.b;
	lay_paths(pr, paths, gm, ptrn);
	pr.a = create_sp(pp.a.r + 1, pp.a.c, pp.a.val + 1);
	lay_paths(pr, paths, gm, ptrn);
	pr.a = create_sp(pp.a.r, pp.a.c - 1, pp.a.val + 1);
	lay_paths(pr, paths, gm, ptrn);
	pr.a = create_sp(pp.a.r, pp.a.c + 1, pp.a.val + 1);
	lay_paths(pr, paths, gm, ptrn);
}

int	alloc_path_structs(int n, int ****c_paths, t_point ****c_points)
{
	*c_paths = (int ***)c_alloc(sizeof(int **) * (n + 1));
	**c_points = (t_point **)c_alloc(sizeof(t_point *) * (n + 1));
	if (!*c_paths || !**c_points)
	{
		c_frr("", *c_paths);
		c_frr("", *c_points);
		return (0);
	}
	return (1);
}

/**
 * Calculates the paths for each corner of the game.
 * @param n         The number of a_corners we want to use
 * @param game      The game.

	* @param c_points  Pointer to the array of t_point which will store the a_corners.

 * @return      A 3D array of integer representing the paths from each corner
 *              to the hero. The last element of the outermost array is set
 *              to NULL as a sentinel.
 *              If memory allocation fails the function returns NULL.
 */
int	***collect_corner_paths(int n, t_game *gm, t_point ***c_points,
		char *pattern_c)
{
	int		i;
	t_point	*stt;
	t_point	*hero;
	int		***c_paths;

	i = 0;
	if (n > 4)
		n = 4;
	if (!alloc_path_structs(n, &c_paths, &c_points))
		return (NULL);
	hero = create_p(gm->hero_r, gm->hero_c, INT_MAX);
	while (i < n)
	{
		(*c_points)[i] = gm->a_corners[i];
		c_paths[i] = init_paths_matrix(gm);
		if (!c_paths[i])
			return (NULL);
		stt = create_p(gm->a_corners[i]->r, gm->a_corners[i]->c, 0);
		lay_paths(create_pp(*stt, *hero), c_paths[i], gm, pattern_c);
		c_frr("", stt);
		i++;
	}
	c_paths[i] = NULL;
	(*c_points)[i] = NULL;
	c_frr("", hero);
	return (c_paths);
}

int	adjust_by_corners(t_point ****c_points, int ****c_paths, t_game *gm,
		char *pattern_c)
{
	t_point	**corn_points;
	int		***corn_paths;
	int		i;

	i = len_p(**c_points);
	if (i < 4)
	{
		corn_points = NULL;
		corn_paths = collect_corner_paths(4 - i, gm, &corn_points, pattern_c);
		if (!corn_paths || !corn_points)
			return (0);
		*c_paths = concat_paths(*c_paths, corn_paths);
		**c_points = concat_p(**c_points, corn_points);
	}
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
int	***collect_exit_paths(t_game *gm, t_point ***c_points, char *pattern_c)
{
	int		i;
	t_point	*stt;
	t_point	*hero;
	int		***c_paths;

	i = 0;
	if (!alloc_path_structs(len_p(gm->a_exits), &c_paths, &c_points))
		return (NULL);
	hero = create_p(gm->hero_r, gm->hero_c, INT_MAX);
	while (gm->a_exits[i])
	{
		(*c_points)[i] = gm->a_exits[i];
		c_paths[i] = init_paths_matrix(gm);
		if (!*c_paths[i])
			return (NULL);
		stt = create_p(gm->a_exits[i]->r, gm->a_exits[i]->c, 0);
		lay_paths(create_pp(*stt, *hero), c_paths[i], gm, pattern_c);
		c_frr("", stt);
		i++;
	}
	c_paths[i] = NULL;
	(*c_points)[i] = NULL;
	// adjust_by_corners(&c_points, &c_paths, gm, pattern_c);
	c_frr("", hero);
	return (c_paths);
}
/**
 * Calculates the paths for each collectible not yet collected in the
 * game to reach the hero.
 * is cunter intuitive but are the paths for the hero to reach the
 * collectibles in reverse.
 *
 * @param game      The game.
 * @param c_points  Pointer to the array of t_point which will store the colls.
 * @param pattern_c The string that defines which characters are acceptable
 *                  for parsing the map.
 * @return      A 3D array of integer representing the paths from each coll
 *              to the hero. The last element of the outermost array is set
 *              to NULL as a sentinel.
 *              If memory allocation fails the function returns NULL.
 */
int	***collect_coll_paths(t_game *gm, t_point ***c_points, char *pattern_c)
{
	int		i;
	int		j;
	t_point	*stt;
	t_point	*hero;
	int		***c_paths;

	i = 0;
	j = 0;
	if (!alloc_path_structs(len_p(gm->a_colls), &c_paths, &c_points))
		return (NULL);
	j = 0;
	hero = create_p(gm->hero_r, gm->hero_c, INT_MAX);
	while (gm->a_colls[j])
	{
		if (gm->a_colls[j]->val == 0)
		{
			(*c_points)[i] = gm->a_colls[j];
			c_paths[i++] = init_paths_matrix(gm);
			if (!c_paths[i - 1])
				return (NULL);
			stt = create_p(gm->a_colls[j]->r, gm->a_colls[j]->c, 0);
			lay_paths(create_pp(*stt, *hero), c_paths[i - 1], gm, pattern_c);
			c_frr("", stt);
		}
		j++;
	}
	c_paths[i] = NULL;
	(*c_points)[i] = NULL;
	// adjust_by_corners(&c_points, &c_paths, gm, pattern_c);
	c_frr("", hero);
	return (c_paths);
}

/**
 * Calculates the paths for each villain in the game to reach the hero.
 *
 * @param       game    The game.
 *
 * @return      A 3D array of integer representing the paths from each vill
 *              to the hero. The last element of the outermost array is set
 *              to NULL as a sentinel.
 *              If memory allocation fails the function returns NULL.
 */
int	***collect_vill_paths(t_game *game)
{
	int		***v_paths;
	int		i;
	t_point	*stt;
	t_point	*hero;

	i = 0;
	hero = create_p(game->hero_r, game->hero_c, INT_MAX);
	v_paths = (int ***)c_alloc(sizeof(int **) * (game->vills + 1));
	if (!v_paths)
	{
		c_frr("", v_paths);
		return (NULL);
	}
	while (i < game->vills)
	{
		v_paths[i++] = init_paths_matrix(game);
		if (!v_paths[i - 1])
			return (NULL);
		stt = create_p(game->a_vills[i - 1]->r, game->a_vills[i - 1]->c, 0);
		lay_paths(create_pp(*stt, *hero), v_paths[i - 1], game, "E1");
		c_frr("", stt);
	}
	v_paths[i] = NULL;
	c_frr("", hero);
	return (v_paths);
}

/**
 * Is a companion to the function trak_back_paths:
 * once track_back_path finds the next step in the path this creates the point
 * structure for it and frees the old one.
 */
static t_point	*f_n_b(t_point **src, t_point *p)
{
	if (p == NULL)
		return (NULL);
	c_frr("", *src);
	*src = p;
	return (p);
}

/**
 * @brief Given a target position and a map path, using the "breadcrumbs"
 * there computed by the lay_paths function against a starting point in
 * the map, returns the next step in the shortest path toward that point
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
 *              1 will trackback, 0 will not and stop at the smallest point
 * @return Pointer to the `t_point` that represents the next move.
 *
 * @remark The input `t_point` struct `t` is freed in each recursive call.
 * @remark each f_n_b call frees the old point and creates a new one. The
 * return (NULL) in the if's is to deal with a failure in such creation.
 * @remark When used in the not track back mode it will return not only
 * the point with the smallest distance (p->val) but also the direction
 * of the move (0=up, 1=right, 2=down, 3=left)
 */
t_point	*trak_back_paths(t_point *t, int **paths, t_game *game/*int tb*/)
{
	t_point	*next_p;

	next_p = create_p(t->r, t->c, paths[t->r][t->c]);
	if (t->r > 0 && paths[t->r - 1][t->c] < next_p->val && !f_n_b(&next_p,
			create_p2(t->r - 1, t->c, paths[t->r - 1][t->c], 0)))
		return (NULL);
	if (t->r < game->map_h - 1 && paths[t->r + 1][t->c] < next_p->val
		&& !f_n_b(&next_p, create_p2(t->r + 1, t->c, paths[t->r + 1][t->c], 2)))
		return (NULL);
	if (t->c > 0 && paths[t->r][t->c - 1] < next_p->val && !f_n_b(&next_p,
			create_p2(t->r, t->c - 1, paths[t->r][t->c - 1], 1)))
		return (NULL);
	if (t->c < game->map_w - 1 && paths[t->r][t->c + 1] < next_p->val
		&& !f_n_b(&next_p, create_p2(t->r, t->c + 1, paths[t->r][t->c + 1], 1)))
		return (NULL);
	if (next_p->val == 0)
	{
		c_frr("", next_p);
		return (t);
	}
	c_frr("", t);
	if (next_p->val == INT_MAX)
		return (NULL);
	// if (!tb)
	// 	return (next_p);
	return (trak_back_paths(next_p, paths, game/*, 1*/));
}

/**
 * Computes the next best move from the position start
 * to the position end within the game map.
 * If a move is not possible (or something happens in the middle),
 * it returns NULL.
 * We make a copy of start because lay_paths will c_frr the corresponding
 * point (as well as the t_point_pair carrying it).
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
t_point	*find_shortest_path(t_point *start, t_point *end, t_game *game/*, int tb*/)
{
	int				**paths;
	t_point_pair	pr;
	t_point			*tmp;
	t_point			*ret;
	int				min;

	paths = init_paths_matrix(game);
	if (!paths)
		return (NULL);
	tmp = create_p(end->r, end->c, INT_MAX);
	pr = create_pp(create_sp(start->r, start->c, 0), *tmp);
	lay_paths(pr, paths, game, "E1");
	ret = trak_back_paths(tmp, paths, game/*, tb*/);
	free(tmp);
	free_paths(paths);
	return (ret);
}

/**
 * Calculates the shortest distance between two points without any
 * consideration on what the obstacles might be in the way, beside walls.
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
t_point	*find_distance(t_point *start, t_point *end, t_game *game)
{
	t_point	*t;

	//t = find_shortest_path(start, end, game, 0);
	if(!t)
		return (NULL);
	if (t->val == INT_MAX)
		t->val = 0;
	return (t);
}

/**
 * Given A, B, C it will find out if a point  A will be able to reach a point
 * C before another moving object B will be able to.  These are scenarios
 * where A will be able to reach C safely before to get caught by C
 * Think of a one way street where we have a car A moving faster than the
 * car B. Both must collect something at point C (target). I need to calculate
 * if A will be able to reach C and turn back before to crash on B
 * B          A
 *  A     AND  C
 *   C          B
 * if BC > BC && AB < BC || AB > AC && CB < AB
 * if AB < AC && BC < AC/1.5 (we factor in that ghosts are slower)
 * we decide there will be a crash.
 (0=up, 1=right, 2=down, 3=left)
 * case 1) B A C
 * if B is moving in the same direction as A it will never catch
 * case 2) A C B
 * if B is moving in the opposite direction as A but AB>AC it will catch up only if BC < AC/1.5
 * case 3) A B C
 * if B is moving in the opposite direction as B but AB<AC better go away
 * case 4)
 * if B is moving in any other direction i would say "maybe". That means if at the next interaction we find out that B is closer
 * maybe is reaching A from another angle. But maybe is quite safe to say BC<AC maybe is not so safe to pursuit
 */
int	will_it_crash(t_point *a, t_point *b, t_point *c, t_game *gm)
{
	t_point	*ac;
	t_point	*ba;
	t_point	*bc;
	int	ret;

	if(ba->val2 == ac->val2)

	ac = find_distance(a, b, gm);
	ba = find_distance(a, c, gm);
	bc = find_distance(b, c, gm);

    // Case 1: B A C
    if (ac->val < bc->val && ac->val2 == ba->val2)
        return 0;
    // Case 2: A C B
    else if (ba->val > bc->val && ba->val2 == bc->val2 && bc->val < (ac->val / 1.5))
		return 1;
	// Case 3) A B C
	else if (ba->val < ac->val && ba->val2 == (bc->val2 + 2) %4)
		return 1;
	else if(ac->val < bc->val)
	//else if(ba->val < ac->val && bc->val < ac->val / 1.5)
		return 1;
	return 0;
}

//     // Case 3: Any other direction
//     if (input.dirB != input.dirA && input.dirB != (input.dirA + 2) % 4) {
//         if (input.BC < input.AC) {
//             return "Maybe (B is approaching)";
//         }
//     }

//     return "Go to C (safe)";
// }
// 	//Scenario A B C

// 	//ret = !(bc > ac && ab < bc || ab > ac && bc < ab);
// 	if(ab->val2 == ac->val2 && ac->val2 == bc->val2)
// 	ret = ab < ac && bc < ac / 1.5;
// 	// printf("AB %i AC %i BC %i RET %i\n", ab, ac, bc, ret);
// 	// printf("MC %i\n", manhtn_crash(a, b, c));
// 	return (ret);
// }

/**
 * @brief Finds the a new object to target in the possible list of objects
 * to track down.
 * First we find out what paths to reach the targets (either collectibles
 * or villains) contained within the variable pts are the shortest
 * by sorting out the 4 cardinals around the current point p from the
 * quickest to the slowest, using them in a manner specular to that used
 * by trak_back_paths.
 * t_point curr is used inappropriately to hold a list of the last target
 * examined so to not pursue always the same object. The one newly
 * selected will be held in the r value of the point.
 */
t_point	*sel_new_tgt_obj(int ***pts, t_point *p, t_point *curr)
{
	t_point	**movs;
	int		i;

	i = 0;
	while (pts[i] != NULL)
		i++;
	movs = (t_point **)c_alloc(sizeof(t_point *) * (i * 4 + 1));
	i = 0;
	while (pts[i] != NULL)
	{
		// printf("curr_path:%i\n", i);
		// printf("%s", mtostr(pts[i], 16, 29));
		movs[i * 4] = create_p2(p->r - 1, p->c, pts[i][p->r - 1][p->c], i);
		movs[i * 4 + 1] = create_p2(p->r + 1, p->c, pts[i][p->r + 1][p->c], i);
		movs[i * 4 + 2] = create_p2(p->r, p->c - 1, pts[i][p->r][p->c - 1], i);
		movs[i * 4 + 3] = create_p2(p->r, p->c + 1, pts[i][p->r][p->c + 1], i);
		i++;
	}
	movs[i * 4] = NULL;
	sort_p(movs, 4 * i);
	i = 0;
	while (movs[i] != NULL && p_contains(curr, movs[i]->val2))
		i++;
	p_shift(curr, movs[i]->val2);
	free_p(movs);
	return (curr);
}

/**
 * @brief       Determines the best path for a hero character in the game.
 *
 * This function finds out if is possible to reach a collectible given the
 * current position of the hero and that of the collectible.
 * The current collectible (as mentioned in sel_new_tgt_obj) is held in the
 * r value of game->tgt_el as a position in the c_points array of collectible
 * coordinates calculated within the calling function.
 * It then uses Manhattan geometry principles to find if the closest
 * approaching villain will be able to reach the hero before it reaches the
 * collectible. If so the calling function will try to find another collectible
 * to go to. The potential crash condition is calculated by passing back the
 * result of manhtn_crash within the val value of the returnd t_point.
 * The returned t_point will b the actual coordinates of the next step in the
 * jurney toward the collectible.
 *
 * @param[in]   gm          The game.
 * @param[in]   c_points    Array of collectible coordinates (or exits when
 *                          the collectibles are exausted)
 * @param[in]   v_paths     The paths toward the hero of all the villains in
 *                          the field from where we select the closest only.
 * @param[in]   pattern_c   String used to lay down a valid path toward a
 *                          collectible as per the lay_paths function
 *
 * @return      A pointer to a t_point which represents the best path for the
 *              hero from the it's current position to reach the collectible
 *              indexed by gm->tgt_el->r. The 'val' field of this t_point
 *              contains the Manhattan crash result test with the closest
 *              approaching villain.
 *
 * Internal workings:
 * The function first initializes a paths matrix. It then creates a start point
 * for the current target element (coll or exit) and an end point for the hero.
 * and fills the path using lay_paths. It then select as target object the
 * closest villain and computes the potential collision between the hero, and
 * the villain while reaching for the current target.
 */
t_point	*is_path_to_curr_tgt_safe(t_game *gm, t_point **c_points,
		int ***v_paths, char *pattern_c)
{
	int		**curr_tgt_path;
	t_point	*stt;
	t_point	*end;
	t_point	*target_v;
	int		d;

	d = gm->colls;
	curr_tgt_path = init_paths_matrix(gm);
	if (!curr_tgt_path)
		return (NULL);
	end = create_p(c_points[gm->tgt_el->r]->r, c_points[gm->tgt_el->r]->c,
		INT_MAX);
	stt = create_p(gm->hero_r, gm->hero_c, 0);
	lay_paths(create_pp(*stt,*end), curr_tgt_path, gm, pattern_c);
	// printf("curr_tgt_path\n");
	// printf("%s",mtostr(curr_tgt_path, gm->map_h, gm->map_w));
	end = trak_back_paths(end, curr_tgt_path, gm/*, 1*/);
	// print_point(end,"Best Move");
	// print_point(gm->tgt_el,"Current Target");
	target_v = sel_new_tgt_obj(v_paths, end, create_p(-1, -1, -1));
	// end->val = will_it_crash(stt, gm->a_vills[target_v->r],
	// 	c_points[gm->tgt_el->r], gm);
	end->val = manhtn_crash(stt, gm->a_vills[target_v->r],
		c_points[gm->tgt_el->r]);
	c_frr("", stt);
	c_frr("", target_v);
	free_paths(curr_tgt_path);
	return (end);
}

/**
 * We need to find the best move for the hero.
 * to do it we'll find all the possible next best moves for each villain
 * and the next possible best move of the hero toward a collectible first
 * and the exit later.
 * so we produce all those mappings (all the vills, all the colls not
 * taken yet, the exits) with lay_paths and we sort
 * them by the shortest path in the four cardinals (up, down, left, right)
 * around the hero using sel_new_tgt_obj. Then with find_best_hero_move we
 * select the next best move.
 */
t_point	*find_hero_move(t_game *gm)
{
	int		***v_paths;
	int		***c_paths;
	t_point	**c_points;
	char	*pattern_c;
	t_point	*hero;

	v_paths = collect_vill_paths(gm); // If NULL
	if (gm->colls > 0)
	{
		pattern_c = "EV1";
		c_paths = collect_coll_paths(gm, &c_points, pattern_c); // If NULL
	}
	else
	{
		pattern_c = "V1";
		c_paths = collect_exit_paths(gm, &c_points, pattern_c); // IF NULL
	}
	hero = create_p(gm->hero_r, gm->hero_c, INT_MAX);
	if (gm->tgt_el->r == -1)
		gm->tgt_el = sel_new_tgt_obj(c_paths, hero, gm->tgt_el);
	hero = is_path_to_curr_tgt_safe(gm, c_points, v_paths, pattern_c);
	while (hero->val)
	{
		gm->tgt_el = sel_new_tgt_obj(c_paths, hero, gm->tgt_el);
		c_frr("", hero);
		hero = is_path_to_curr_tgt_safe(gm, c_points, v_paths, pattern_c);
	}
	// what the hack is going on here?!!!
	free_paths_matrix(c_paths);
	free_paths_matrix(v_paths);
	// free_p(c_points);
	return (hero);
}
/**
 * Shifts an array and inserts a new head;
 *
 * @param stack  The stack to be rotated.
 * @param head   The value to insert at the head(0) of the stack
 * @param length The length of the stack.
 */
void	shift(int *stack, int head, int length)
{
	int	i;

	i = length - 1;
	while (i > 0)
	{
		stack[i] = stack[i - 1];
		i--;
	}
	stack[0] = head;
}
