/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ap_utils_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 03:16:48 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/22 00:29:13 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

t_point	**rev_sort_ap(t_point **arr)
{
	return (rev_ap(sort_ap(arr)));
}

void	free_ap(t_point **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i] != NULL)
		free(arr[i++]);
	free(arr);
}

int	len_ap(t_point **p)
{
	int	i;

	i = 0;
	while (p[i] != NULL)
		i++;
	return (i);
}
