/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ap_utils2_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 07:26:49 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/17 03:21:07 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

static void	swap_sort(t_point **arr, int j)
{
	t_point	*temp;

	temp = arr[j];
	arr[j] = arr[j + 1];
	arr[j + 1] = temp;
}

/**
 * sorts an array of p ascending. Returns the value to save some space
 * somewhere else
 */
t_point	**sort_ap(t_point **arr, int n)
{
	int	i;
	int	j;

	i = 0;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - i - 1)
		{
			if (arr[j]->val > arr[j + 1]->val)
				swap_sort(arr, j);
			else if (arr[j]->val == arr[j + 1]->val)
				if (arr[j]->val2 > arr[j + 1]->val2)
					swap_sort(arr, j);
			j++;
		}
		i++;
	}
	return (arr);
}

t_point	**rev_ap(t_point **arr, int n)
{
	int		i;
	t_point	*temp;

	i = 0;
	n -= 1;
	while (i < n)
	{
		temp = arr[i];
		arr[i] = arr[n];
		arr[n] = temp;
		i++;
		n--;
	}
	return (arr);
}
