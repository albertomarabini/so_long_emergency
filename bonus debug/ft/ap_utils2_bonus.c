/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ap_utils2_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 07:26:49 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/22 06:47:46 by amarabin         ###   ########.fr       */
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
 * sorts an array of p ascending by the val, val2 values.
 *Returns the value to save some space somewhere else but
 * the array is sorted in place.
 */
t_point	**sort_ap(t_point **arr)
{
	int	i;
	int	j;
	int	n;

	n = len_ap(arr);
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

t_point	**rev_ap(t_point **arr)
{
	int		i;
	t_point	*temp;
	int		n;

	n = len_ap(arr);
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

int	avg_ap(t_point **arr)
{
	int	sum;
	int	count;
	int	i;

	sum = 0;
	count = 0;
	i = 0;
	while (arr[i++])
	{
		if (arr[i - 1]->val != INT_MAX)
		{
			sum += arr[i - 1]->val;
			count++;
		}
	}
	if (count == 0)
		return (0);
	return (sum / count);
}

t_point	**sort_avg_ap(t_point **arr)
{
	int	n;
	int	avg;
	int	j;
	int	i;

	avg = avg_ap(arr);
	n = len_ap(arr);
	i = 0;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - i - 1)
		{
			if (abs(arr[j]->val - avg) > abs(arr[j + 1]->val - avg))
				swap_sort(arr, j);
			j++;
		}
		i++;
	}
	return (arr);
}
