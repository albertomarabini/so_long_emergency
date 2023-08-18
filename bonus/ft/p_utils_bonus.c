/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_utils_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 08:25:31 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/17 04:23:08 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

t_point	duplicate_p(t_point p)
{
	t_point	point;

	point.r = p.r;
	point.c = p.c;
	point.val = p.val;
	point.val2 = p.val2;
	return (point);
}

/**
 * creates a new point pointer with the given row, column and value.
 */
t_point	*create_pp(int r, int c, int val, int val2)
{
	t_point	*point;

	point = (t_point *)c_alloc(NULL, sizeof(t_point));
	point->r = r;
	point->c = c;
	point->val = val;
	point->val2 = val2;
	return (point);
}

/**
 * creates a new point pointer with the data of the passed point.
 */
t_point	*p_to_pp(t_point p)
{
	t_point	*point;

	point = (t_point *)c_alloc(NULL, sizeof(t_point));
	point->r = p.r;
	point->c = p.c;
	point->val = p.val;
	point->val2 = p.val2;
	return (point);
}
