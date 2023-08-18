/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_utils2_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 08:25:31 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/17 03:48:08 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

/**
 * creates a new point with the given row, column and value.
 */
t_point	create_p(int r, int c, int val, int val2)
{
	t_point	point;

	point.r = r;
	point.c = c;
	point.val = val;
	point.val2 = val2;
	return (point);
}

t_point	null_p(void)
{
	t_point	point;

	point.r = INT_MAX;
	point.c = INT_MAX;
	point.val = INT_MAX;
	point.val2 = INT_MAX;
	return (point);
}

int	is_null_p(t_point p)
{
	if (p.r == INT_MAX && p.c == INT_MAX && p.val == INT_MAX
		&& p.val2 == INT_MAX)
		return (1);
	return (0);
}
