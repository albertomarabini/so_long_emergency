/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_path_finder_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 10:47:19 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/11 19:23:32 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

/* DEBUG */
// printf("curr_tgt_path\n");
// printf("%s",mtostr(curr_tgt_path, gm->map_h, gm->map_w));
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
int	**init_paths_matrix(t_game *game)
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

int	len_paths_m(int ***paths)
{
	int	i;

	i = 0;
	while (paths[i] != NULL)
		i++;
	return (i);
}

typedef struct s_pair
{
	t_point		a;
	t_point		b;
}				t_pair;

// Function to instantiate a t_pair
t_pair	create_pp(t_point a, t_point b)
{
	t_pair	pair;

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

void print_array(int arr[], int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}


/**
 *we are using a inappropriately using a point as a collection of values
 */
int	p_contains(t_point p, int val)
{
	return (p.r == val || p.c == val || p.val == val);
}
t_point	p_shift(t_point p, int val)
{
	p.val = p.c;
	p.c = p.r;
	p.r = val;
	return (p);
}

/**
 * Shifts an array and inserts a new head;
 *
 * @param a  The array to be rotated.
 * @param head   The value to insert at the head(0) of the array
 * @param length The length of the array.
 */
void	a_shift(int *a, int head, int length)
{
	int	i;

	i = length - 1;
	while (i > 0)
	{
		a[i] = a[i - 1];
		i--;
	}
	a[0] = head;
}

/**
 *  Inserts 'value' inside an array;
 *
 * @param a  The array to be nulled.
 * @param length The length of the array.
 */
void	set_a(int *a, int length, int value)
{
	int	i;

	i = 0;
	while (i < length)
	{
		a[i] = value;
		i++;
	}
}

/**
 * Inserts all INT_MAX inside an array;
 *
 * @param a  The array to be nulled.
 * @param length The length of the array.
 */
void	null_a(int *a, int length)
{
	set_a(a,length,INT_MAX);
}

/**
 * returns the index of the highest value in a
 *
 * @param a  The array to be nulled.
 * @param length The length of the array.
 */
int find_max_a(int a[], int length)
{
    int max = 0;
	int i =0;

    while(i < length)
	{
        if (a[i] > a[max]) {
            max = i;
        }
		i++;
    }
    return (max);
}
/**
 * Verifies that an array is nulled
 *
 * @param a  The array to be rotated.
 * @param length The length of the array.
 */
int	a_is_null(int *a, int length)
{
	int	i;

	i = 0;
	while (i < length)
	{
		if (a[i] != INT_MAX)
			return (0);
		i++;
	}
	return (1);
}

/**
 * Verifies that an array contains a value
 *
 * @param a  The array to be rotated.
 * @param val The value to be checked.
 * @param length The length of the array.
 */
int	a_contains(int *a, int val, int length)
{
	int	i;

	i = 0;
	while (i < length)
	{
		if (a[i] == val)
			return (1);
		i++;
	}
	return (0);
}

/**
 * Duplicates an array
 *
 * @param a  The array to be duplicated.
 * @param b  The array destination
 * @param length The length of the arrays.
 */
void	a_dup(int *a, int *b, int length)
{
	int	i;

	i = 0;
	while (i < length)
	{
		b[i] = a[i];
		i++;
	}
}

/**
 * Compares two arrays to see if they have the same values in the same order
 *
 * @param a  The array to be duplicated.
 * @param b  The array destination
 * @param length The length of the arrays.
 */
int	a_cmp(int *a, int *b, int length)
{
	int	i;

	i = 0;
	while (i < length)
	{
		if (a[i] != b[i])
			return (0);
		i++;
	}
	return (1);
}

int	abs(int val)
{
	if (val < 0 && val > INT_MIN)
		return (-val);
	return (val);
}
/**
 * checks whether number a is within bound units from number b
 */
int within(int a, int b, int bound) {
    return abs(a - b) <= bound;
}

int	len_oo(void **o)
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

t_point	clone_p(t_point *p)
{
	t_point	clone;

	clone.r = p->r;
	clone.c = p->c;
	clone.val = p->val;
	clone.val2 = p->val2;
	return (clone);
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
int	manhtn_dst(t_point p1, t_point p2)
{
	return (abs(p1.r - p2.r) + abs(p1.c - p2.c));
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
int	manhtn_crash(t_point a, t_point b, t_point c)
{
	t_point	tmp;
	int		ret;

	tmp = create_sp(manhtn_dst(a, b), manhtn_dst(a, c), manhtn_dst(b, c));
	ret = tmp.r < tmp.c && tmp.val < tmp.c / 1.5;
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
	l1 = len_oo((void **)p1);
	l2 = len_oo((void **)p2);
	pc = (int ***)c_alloc(sizeof(int **) * (l1 + l2 + 1));
	ft_memcpy(pc, p1, l1 * sizeof(int **));
	ft_memcpy(pc + l1, p2, l2 * sizeof(int **));
	pc[l1 + l2] = NULL;
	c_frr("", p1);
	c_frr("", p2);
	return (pc);
}

char **duplicate_map(t_game *gm) {
	int i;
    char **new_map = (char **)malloc((gm->map_h + 1) * sizeof(char *));
	if (new_map == NULL) {
		// Handle memory allocation failure
    }
	new_map[gm->map_h] = NULL;
	i = 0;
    while(i < gm->map_h) {
        new_map[i] = strdup(gm->map[i]);
        if (new_map[i] == NULL) {
            // Handle memory allocation failure
        }
		i++;
    }
    return new_map;
}

void fill_map_unsafe_spots(char **map, int r, int c, int ray) {
    if (map[r][c] == '1' || ray == 0)
        return;
    map[r][c] = '1';

    fill_map_unsafe_spots(map, r-1, c, ray-1);
    fill_map_unsafe_spots(map, r+1, c, ray-1);
    fill_map_unsafe_spots(map, r, c-1, ray-1);
    fill_map_unsafe_spots(map, r, c+1, ray-1);
}

t_game *create_safe_map(t_game *gm, int ray)
{
	t_game *safe;
	int r;
	int c;

	safe = (t_game *)c_alloc(sizeof(t_game));
	if (!safe)
		return (NULL);
	safe->map = duplicate_map(gm);
	r = 0;
    while(r < gm->map_h) {
		c = 0;
        while(c < gm->map_w) {
			if(safe->map[r][c] == 'V')
				fill_map_unsafe_spots(safe->map, r, c, ray);
			c++;
		}
		r++;
    }
	safe->map_h = gm->map_h;
	safe->map_w = gm->map_w;
	safe->hero_c = gm->hero_c;
	safe->hero_r = gm->hero_r;
	safe->a_corners = gm->a_corners;
	safe->a_vills = gm->a_vills;
	null_a(safe->tgt_cr, 3);
	return (safe);
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
 * @remark 'V' positions are going to be replaced in the map once villains move
 */
static void	lay_paths(t_pair p, int **pth, t_game *gm, char *ptrn)
{
	t_pair	pr;

	if (p.a.val > 0)
	{
		if ((p.a.r < 1 || p.a.r > gm->map_h - 2 || p.a.c < 1
				|| p.a.c > gm->map_w - 2) || strchr(ptrn, gm->map[p.a.r][p.a.c])
			|| p.a.val >= pth[p.a.r][p.a.c] || (p.a.r == p.b.r
				&& p.a.c == p.b.c))
		{
			if (p.a.r == p.b.r && p.a.c == p.b.c && p.a.val < pth[p.a.r][p.a.c])
				pth[p.a.r][p.a.c] = p.a.val;
			return ;
		}
	}
	pth[p.a.r][p.a.c] = p.a.val;
	pr.a = create_sp(p.a.r - 1, p.a.c, p.a.val + 1);
	pr.b = p.b;
	lay_paths(pr, pth, gm, ptrn);
	pr.a = create_sp(p.a.r + 1, p.a.c, p.a.val + 1);
	lay_paths(pr, pth, gm, ptrn);
	pr.a = create_sp(p.a.r, p.a.c - 1, p.a.val + 1);
	lay_paths(pr, pth, gm, ptrn);
	pr.a = create_sp(p.a.r, p.a.c + 1, p.a.val + 1);
	lay_paths(pr, pth, gm, ptrn);
}

int	alloc_path_structs(int n, int ****c_paths_m, t_point ****c_points)
{
	*c_paths_m = (int ***)c_alloc(sizeof(int **) * (n + 1));
	**c_points = (t_point **)c_alloc(sizeof(t_point *) * (n + 1));
	if (!*c_paths_m || !**c_points)
	{
		c_frr("", *c_paths_m);
		c_frr("", *c_points);
		return (0);
	}
	return (1);
}

/**
 * Calculates the paths for each corner of the game.
 * @param n         The number of a_corners we want to use
 * @param game      The game.
 * @param c_points  Pointer to the array of t_point which will store the
 *                  a_corners.
 * @return      A 3D array of integer representing the paths from each corner
 *              to the hero. The last element of the outermost array is set
 *              to NULL as a sentinel.
 *              If memory allocation fails the function returns NULL.
 */
int	***collect_corner_paths(int n, t_game *gm, t_point ***c_points,
		char *pattern_c)
{
	int		i;
	t_point	stt;
	t_point	hero;
	t_point	*c;
	int		***c_paths_m;

	i = 0;
	if (n > 4)
		n = 4;
	if (!alloc_path_structs(n, &c_paths_m, &c_points))
		return (NULL);
	hero = create_sp(gm->hero_r, gm->hero_c, 0);
	while (i < n)
	{
		c = gm->a_corners[i];
		(*c_points)[i] = create_p(c->r, c->c, c->val);
		c_paths_m[i] = init_paths_matrix(gm);
		if (!c_paths_m[i])
			return (NULL);
		stt = create_sp(gm->a_corners[i]->r, gm->a_corners[i]->c, 0);
		lay_paths(create_pp(stt, hero), c_paths_m[i], gm, pattern_c);
		i++;
	}
	c_paths_m[i] = NULL;
	(*c_points)[i] = NULL;
	return (c_paths_m);
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
	int		***c_paths_m;

	i = 0;
	if (!alloc_path_structs(len_p(gm->a_exits), &c_paths_m, &c_points))
		return (NULL);
	hero = create_p(gm->hero_r, gm->hero_c, INT_MAX);
	while (gm->a_exits[i])
	{
		(*c_points)[i] = create_p2(gm->a_exits[i]->r, gm->a_exits[i]->c,
			gm->a_exits[i]->val, gm->a_exits[i]->val2);
		c_paths_m[i] = init_paths_matrix(gm);
		if (!*c_paths_m[i])
			return (NULL);
		stt = create_p(gm->a_exits[i]->r, gm->a_exits[i]->c, 0);
		lay_paths(create_pp(*stt, *hero), c_paths_m[i], gm, pattern_c);
		c_frr("", stt);
		i++;
	}
	c_paths_m[i] = NULL;
	(*c_points)[i] = NULL;
	c_frr("", hero);
	return (c_paths_m);
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
	int		***c_paths_m;

	i = 0;
	j = 0;
	if (!alloc_path_structs(len_p(gm->a_colls), &c_paths_m, &c_points))
		return (NULL);
	j = 0;
	hero = create_p(gm->hero_r, gm->hero_c, INT_MAX);
	while (gm->a_colls[j])
	{
		if (gm->a_colls[j]->val == 0)
		{
			(*c_points)[i] = create_p2(gm->a_colls[j]->r, gm->a_colls[j]->c,
				gm->a_colls[j]->val, gm->a_colls[j]->val2);
			c_paths_m[i++] = init_paths_matrix(gm);
			if (!c_paths_m[i - 1])
				return (NULL);
			stt = create_p(gm->a_colls[j]->r, gm->a_colls[j]->c, 0);
			lay_paths(create_pp(*stt, *hero), c_paths_m[i - 1], gm, pattern_c);
			c_frr("", stt);
		}
		j++;
	}
	c_paths_m[i] = NULL;
	(*c_points)[i] = NULL;
	c_frr("", hero);
	return (c_paths_m);
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
	int		***v_paths_m;
	int		i;
	t_point	*stt;
	t_point	*hero;

	i = 0;
	hero = create_p(game->hero_r, game->hero_c, INT_MAX);
	v_paths_m = (int ***)c_alloc(sizeof(int **) * (game->vills + 1));
	if (!v_paths_m)
	{
		c_frr("", v_paths_m);
		return (NULL);
	}
	while (i < game->vills)
	{
		v_paths_m[i++] = init_paths_matrix(game);
		if (!v_paths_m[i - 1])
			return (NULL);
		stt = create_p(game->a_vills[i - 1]->r, game->a_vills[i - 1]->c, 0);
		lay_paths(create_pp(*stt, *hero), v_paths_m[i - 1], game, "E1");
		c_frr("", stt);
	}
	v_paths_m[i] = NULL;
	c_frr("", hero);
	return (v_paths_m);
}

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

	next_p = create_sp(t.r, t.c, paths[t.r][t.c]);
	if (t.r > 0 && paths[t.r - 1][t.c] < next_p.val)
		next_p = create_sp2(t.r - 1, t.c, paths[t.r - 1][t.c], 0);
	if (t.r < game->map_h - 1 && paths[t.r + 1][t.c] < next_p.val)
		next_p = create_sp2(t.r + 1, t.c, paths[t.r + 1][t.c], 2);
	if (t.c > 0 && paths[t.r][t.c - 1] < next_p.val)
		next_p = create_sp2(t.r, t.c - 1, paths[t.r][t.c - 1], 1);
	if (t.c < game->map_w - 1 && paths[t.r][t.c + 1] < next_p.val)
		next_p = create_sp2(t.r, t.c + 1, paths[t.r][t.c + 1], 1);
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
 * Computes the next best move from the position start
 * to the position end within the game map.
 * If a move is not possible (or something happens in the middle),
 * it returns NULL.
 * We make a copy of start because lay_paths will c_frr the corresponding
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
	int		min;

	path = init_paths_matrix(game);
	if (!path)
		return (null_p());
	pr = create_pp(create_sp(start.r, start.c, 0), end);
	lay_paths(pr, path, game, "E1");
	nxt = trak_back_paths(end, path, game, tb);
	free_paths(path);
	return (nxt);
}

/**
 * Calculates the shortest distance between two points without any
 * consideration on what the obstacles might be in the way, beside walls.
 * val contains the distance, r and c the vert and horiz displacements
 *  A
 *  |\
 * r| \ val
 *  |  \
 *   ---B
 *    c
 * It also return the direction that has to be taken to reach end from start
 * (0=up, 1=right, 2=down, 3=left) in the val2 field.
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
	t.r = abs (start.r - end.r);
	t.c = abs (start.c - end.c);
	if (is_null_p(t))
		return (t);
	if (t.val == INT_MAX)
		t.val = 0;
	return (t);
}

/**
 * Given A, B, T it will find out if smthg at point A will be able to reach
 * smthg at point T before another moving object B will be able to.
 * Think of a one way street where we have a car A moving faster than the
 * car B. A must collect something at point T, B is chasing A.
 * We need to calculate if A will be able to reach C and turn back before
 * to crash on B
 * We are going to factor in also the direction
 * (0=up, 1=right, 2=down, 3=left)
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
	int		ret;

	at = find_distance(a, t, gm);
	ba = find_distance(b, a, gm);
	bt = find_distance(b, t, gm);
	if (at.val < bt.val && at.val2 == ba.val2){
		printf("safe reason 1");
		return (0);
	}
	else if (ba.val > bt.val && ba.val > at.val && ba.val2 == bt.val2
		&& bt.val < (at.val / 1.5)){
		printf("unsafe reason 2");
		return (1);
	}
	else if (ba.val < at.val && ba.val2 == (at.val2 + 2) % 4) {
		printf("unsafe reason 3");
		return (1);
	}
	else if (ba.val < at.val || bt.val /** 1.5*/ < at.val / 2) {
		printf("unsafe reason 4");
		return (1);
	}
	printf("safe reason 5");
	return (0);
}

// int	will_it_crash(t_point a, t_point b, t_point t, t_game *gm)
// {
// 	t_point	at;
// 	t_point	ba;
// 	t_point	bt;
// 	int		ret;

// 	at = find_distance(a, t, gm);
// 	ba = find_distance(b, a, gm);
// 	bt = find_distance(b, t, gm);
// 	if (at.val < bt.val && at.val2 == ba.val2)
// 		return (0);
// 	else if (ba.val > bt.val && ba.val > at.val && ba.val2 == bt.val2
// 		&& bt.val < (at.val / 1.5))
// 		return (1);
// 	else if (ba.val < at.val && ba.val2 == (at.val2 + 2) % 4)
// 		return (1);
// 	else if (bt.val < at.val / 1.5)
// 		return (1);
// 	return (0);
// }

/**
 * Creates an array of potential paths to reach the targets (either
 * collectibles or villains) from the variable pts collecting the
 * 4 cardinals around the current point p for each path.
 */
static t_point	**pot_tgt_moves(int ***pts, int len_pts, t_point p)
{
	t_point	**movs;
	int		i;

	movs = (t_point **)c_alloc(sizeof(t_point *) * (len_pts * 4 + 1));
	i = 0;
	while (pts[i] != NULL)
	{
		movs[i * 4] = create_p2(p.r - 1, p.c, pts[i][p.r - 1][p.c], i);
		movs[i * 4 + 1] = create_p2(p.r + 1, p.c, pts[i][p.r + 1][p.c], i);
		movs[i * 4 + 2] = create_p2(p.r, p.c - 1, pts[i][p.r][p.c - 1], i);
		movs[i * 4 + 3] = create_p2(p.r, p.c + 1, pts[i][p.r][p.c + 1], i);
		movs[++i * 4] = NULL;
	}
	return (movs);
}
/**
 * Finds the a new object to target in the possible list of objects
 * to track down.
 * First we find out what paths to reach the targets (either collectibles
 * or villains) contained within the variable pts are the shortest
 * by sorting out the 4 cardinals around the current point p from the
 * quickest to the slowest, using them in a manner specular to that used
 * by trak_back_paths.
 * t_point curr is used inappropriately to hold a list of the last target
 * examined so to not pursue always the same object. The one newly
 * selected will be held in the r value of the point.
 * If none is found it will try to reset the search.
 */
void	sel_new_tgt_obj(int ***pts, t_point p, int (*curr)[3])
{
	t_point	**movs;
	int		i;
	int len_pts;

	len_pts = 0;
	while (pts[len_pts] != NULL)
		len_pts++;
	i = 4 * len_pts;
	movs = pot_tgt_moves(pts, len_pts, p);
	sort_p(movs, i);
	i = 0;
	while (movs[i] != NULL && a_contains(*curr, movs[i]->val2, 3))
		i++;
	if (movs[i] != NULL)
		a_shift(*curr, movs[i]->val2, 3);
	else
	{
		null_a(*curr, 3);
		a_shift(*curr, movs[0]->val2, 3);
	}
	free_p(movs);
}

/**
 * Does the same thing as sel_new_tgt_obj but tailored for corners
 * (we need to find the farthest reacheable, not the closest)
 */
void	sel_new_tgt_corn(int ***pts, t_point p, int (*curr)[3])
{
	t_point	**movs;
	int		i;
	int len_pts;

	len_pts = 0;
	while (pts[len_pts] != NULL)
		len_pts++;
	i = 4 * len_pts;
	movs = pot_tgt_moves(pts, len_pts, p);
	rev_p(sort_p(movs, i), i);
	//sort_p(movs, i);
	i = 0;
	while (movs[i] != NULL)
	{
		if (movs[i]->val != INT_MAX && !a_contains(*curr, movs[i]->val2, 3))
			break ;
		i++;
	}
	if (movs[i] != NULL)
		a_shift(*curr, movs[i]->val2, 3);
	else
		null_a(*curr, 3);
	free_p(movs);
}

/**
 * Uses Manhattan geometry principles to find if the closest
 * approaching villain will be able to reach the hero before it reaches the
 * target (whatever the target might be).
 * @param   t           The game.
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
 * The function first initializes a paths matrix. It then creates a start point
 * point for the hero and fills the path using lay_paths.
 * It then select as target object the closest villains and computes the
 * potential collision with the hero
 */
static t_point	is_path_to_tgt_nsafe(t_point t, t_game *gm, int ***v_paths_m, char *pattern_c)
{
	int		**curr_tgt_path;
	t_point	strt;
	t_point	nxt;
	int		tgt_v[3];
	int i;

	curr_tgt_path = init_paths_matrix(gm);
	if (!curr_tgt_path)
		return (null_p());
	strt = create_sp(gm->hero_r, gm->hero_c, 0);
	lay_paths(create_pp(strt, t), curr_tgt_path, gm, pattern_c);
	nxt = trak_back_paths(t, curr_tgt_path, gm, 1);
	if (is_null_p(nxt))
		return (nxt);
	i = 0;
	null_a(tgt_v, 3);
	while (trn(gm->vills > 4, 4, gm->vills) > i)
	//while(i<2)
	{
		sel_new_tgt_obj(v_paths_m, t, &tgt_v);
		nxt.val = will_it_crash(strt, *(gm->a_vills[tgt_v[0]]), t, gm);
		printf(" vill :%i (%i,%i) crash: %i ", i, gm->a_vills[tgt_v[0]]->r, gm->a_vills[tgt_v[0]]->c,  nxt.val);
		print_array(tgt_v, 3);
		if(nxt.val)
			break;
		i++;
	}
	free_paths(curr_tgt_path);
	return (nxt);
}

/**
 * @brief       Determines the best path for a hero character in the game.
 *
 * This function finds out if is possible to reach a collectible given the
 * current position of the hero and that of the collectible.
 * The current collectible (as mentioned in sel_new_tgt_obj) is held in the
 * r value of game->tgt_el as a position in the c_points array of collectible
 * coordinates calculated within the calling function.
 * If the path is unsafe the calling function will try to find another tgt
 * to go to.
 * The returned t_point will b the actual coordinates of the next step in the
 * jurney toward the collectible.
 *
 * @param[in]   gm          The game.
 * @param[in]   c_points    Array of collectible coordinates (or exits when
 *                          the collectibles are exausted)
 * @param[in]   v_paths_m     The paths toward the hero of all the villains in
 *                          the field from where we select the closest only.
 * @param[in]   pattern_c   String used to lay down a valid path toward a
 *                          collectible as per the lay_paths function
 *
 * @return      A pointer to a t_point which represents the best path for the
 *              hero from the it's current position to reach the collectible
 *              indexed by gm->tgt_el[0]. The 'val' field of this t_point
 *              contains the Manhattan crash result test with the closest
 *              approaching villain.
 *
 * Internal workings:
 * The function creates an end point for the current target element
 * (coll or exit) and pass it to is_path_to_tgt_nsafe to find a safe
 * path to it.
 */
t_point	is_path_to_curr_tgt_nsafe(t_game *gm, t_point **c_points,
		int ***v_paths_m, char *pattern_c)
{
	t_point	end;

	end = create_sp(c_points[gm->tgt_el[0]]->r, c_points[gm->tgt_el[0]]->c, 0);
	end = is_path_to_tgt_nsafe(end, gm, v_paths_m, pattern_c);
	return (end);
}

/**
 * Does the same thing as is_path_to_curr_tgt_nsafe but tailored for corners
 */
t_point	is_path_to_curr_cor_nsafe(t_game *gm, t_point **c_points,
		int ***v_paths_m, char *pattern_c)
{
	t_point	end;

	end = create_sp(c_points[gm->tgt_cr[0]]->r, c_points[gm->tgt_cr[0]]->c, 0);
	end = is_path_to_tgt_nsafe(end, gm, v_paths_m, pattern_c);
	return (end);
}

// t_point	run_hero_run(t_game *gm, int ***v_paths_m)
// {
// 	char	*pattern_c;
// 	t_point	hero;
// 	t_point vill;
// 	t_point corner;
// 	t_point run;
// 	int i;
// 	int j;
// 	int		tgt_v[3];
// 	int     scores[4];
// 	int score;

// 	pattern_c = "EV1";
// 	hero = create_sp(gm->hero_r, gm->hero_c, 0);
// 	i = 0;
// 	null_a(tgt_v, 3);
// 	set_a(scores, 4, 0);
// 	while (trn(gm->vills > 3, 3, gm->vills) > i)
// 	//while(i<1)
// 	{
// 		sel_new_tgt_obj(v_paths_m, len_paths_m(v_paths_m), hero, &tgt_v);
// 		vill = *(gm->a_vills[tgt_v[0]]);
// 		j=0;
// 		while (j<4)
// 		{
// 			t_point approach = find_distance(hero, vill, gm);
// 			run = find_distance(hero, *(gm->a_corners[j]), gm);
// 			printf("hero run corner %i (%i,%i)\n", j, run.r, run.c);
// 			printf("approach %i (%i,%i) run %i\n", approach.val2, approach.r, approach.c, run.val2);

// 			if(approach.val2 == (run.val2 + 2) % 4)
// 				scores[j] += 1000/approach.val;
// 			else if(approach.val2 != run.val2)
// 				scores[j] += 500/approach.val;
// 			j++;
// 		}
// 		i++;
// 	}
// 	i = find_max_a(scores, 4);
// 	gm->run_to = *(gm->a_corners[i]);
// 	run = find_distance(hero, gm->run_to, gm);
// 	j = run.val;
// 	i = trn(i == 2 || i ==0, gm->map_w, gm->map_h)/3;
// 	gm->run = trn(i > j, i , j);
// 	return (find_shortest_path(hero, gm->run_to, gm, 1));
// }




t_point	run_hero_run(t_game *gm, int ***v_paths_m)
{
	int		***c_paths_m;
	t_point	**c_points;
	char	*pattern_c;
	t_game *safe_game;
	t_point	hero;
	t_point	run;
	int i;

	pattern_c = "EV1";
	safe_game = create_safe_map(gm, 2);
	c_paths_m = collect_corner_paths(4, safe_game, &c_points, pattern_c); // If NULL
	hero = create_sp(gm->hero_r, gm->hero_c, 0);
	i = 0;
	gm->run_unsafe = 1;
	while (5 > i++) //4+1
	{
		sel_new_tgt_corn(c_paths_m, hero, &(safe_game->tgt_cr));
		if(within(hero. r, c_points[safe_game->tgt_cr[0]]->r, 2) && within(hero.c, c_points[safe_game->tgt_cr[0]]->c,2))
			continue ;
		hero = is_path_to_curr_cor_nsafe(safe_game, c_points, v_paths_m, pattern_c);
		printf("hero run fnd corner %i(%i,%i) crash:%i ", i - 1, c_points[safe_game->tgt_cr[0]]->r, c_points[safe_game->tgt_cr[0]]->c, hero.val);
		printf("%s",mtostr(c_paths_m[safe_game->tgt_cr[0]], gm->map_h, gm->map_w));
		print_array(safe_game->tgt_cr, 3);
		if(!is_null_p(hero) && !hero.val) {
			gm->run_unsafe = 0;
			break;
		}
	}
	gm->run_to = *(c_points[safe_game->tgt_cr[0]]);
	run = find_distance(hero, gm->run_to, gm);
	gm->run = trn(run.r > run.c,run.r,run.c) + 1;
	null_a(gm->tgt_cr, 3);
	free_paths_matrix(c_paths_m);
	free_p(c_points);
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
 * around the hero using sel_new_tgt_obj. Then with find_best_hero_move we
 * select the next best move.
 */
t_point	find_hero_move(t_game *gm)
{
	int		***v_paths_m;
	int		***c_paths_m;
	t_point	**c_points;
	t_game *safe_game;
	char	*pattern_c;
	t_point	hero;
	int targets;
	int i;

	hero = create_sp(gm->hero_r, gm->hero_c, 0);
	printf("hero: (%i,%i)", hero.r,hero.c);
	v_paths_m = collect_vill_paths(gm); // If NULL
	if(gm->run){
		i = is_path_to_tgt_nsafe(gm->run_to, gm, v_paths_m, "EV1").val;
		// We check if the run was unsafe from the getgo
		// If keep being unsafe there is no reason to bother
		// Instead if this condition has changed better rielaborate
		if (gm->run_unsafe == 0 && i == 1)
			gm->run = 0;
		else
		{
			gm->run--;
			free_paths_matrix(v_paths_m);
			safe_game = create_safe_map(gm, 2);
			return (find_shortest_path(hero, gm->run_to, safe_game, 1));
		}
	}
	if (gm->colls > 0)
	{
		pattern_c = "EV1";
		targets = gm->colls;
		c_paths_m = collect_coll_paths(gm, &c_points, pattern_c); // If NULL
	}
	else
	{
		pattern_c = "V1";
		targets = gm->exits;
		c_paths_m = collect_exit_paths(gm, &c_points, pattern_c); // IF NULL
	}
	i = 0;
	while (trn(targets> 5, 5, targets) > i)
	//while (targets > i)
	{
		if (a_is_null(gm->tgt_el, 3))
			sel_new_tgt_obj(c_paths_m, hero, &(gm->tgt_el));
		printf("target:%i of %i (%i,%i)", i, targets, c_points[gm->tgt_el[0]]->r, c_points[gm->tgt_el[0]]->c);
		print_array(gm->tgt_el, 3);
		hero = is_path_to_curr_tgt_nsafe(gm, c_points, v_paths_m, pattern_c);
		if(!is_null_p(hero) && !hero.val)
			break;
		if (i == trn(targets> 5, 5, targets) - 1)
		//if(i == targets -1)
		{
			hero = run_hero_run(gm, v_paths_m);
			null_a(gm->tgt_el, 3);
			break ;
		}
		i++;
	}
	free_paths_matrix(c_paths_m);
	free_paths_matrix(v_paths_m);
	free_p(c_points);
	return (hero);
}
