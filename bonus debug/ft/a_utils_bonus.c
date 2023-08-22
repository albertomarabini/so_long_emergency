/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_utils_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 07:17:22 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/21 11:37:19 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

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
	set_a(a, length, INT_MAX);
}

/**
 * Verifies that an array is nulled
 *
 * @param a  The array to be rotated.
 * @param length The length of the array.
 */
int	is_null_a(int *a, int length)
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
int	a_contn(int *a, int val, int length)
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
