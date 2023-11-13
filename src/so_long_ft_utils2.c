/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_ft_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 09:16:45 by amarabin          #+#    #+#             */
/*   Updated: 2023/11/13 00:03:23 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

char	*strch(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

size_t	strl(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (s[len])
		len++;
	return (len);
}

static char	**duplicate_map(t_game *gm)
{
	int		i;
	int		k;
	char	**new_map;

	new_map = (char **)malloc((gm->map_h + 1) * sizeof(char *));
	if (!new_map)
		return (NULL);
	new_map[gm->map_h] = NULL;
	i = 0;
	while (gm->map_h > i++)
	{
		new_map[i - 1] = strdp(gm->map[i - 1]);
		if (new_map[i - 1] == NULL)
		{
			k = 0;
			while (k < i - 1)
				free(new_map[k++]);
			free(new_map);
			return (NULL);
		}
	}
	return (new_map);
}

static void	unreach(int r, int c, t_game eval, char **map)
{
	if (r < 0 || c < 0 || r > eval.map_h - 1 || c > eval.map_w - 1)
		return ;
	if (map[r][c] == '1')
		return ;
	if (map[r][c] == 'E')
		eval.exit--;
	if (map[r][c] == 'C')
		eval.colls--;
	map[r][c] = '1';
	unreach(r + 1, c, eval, map);
	unreach(r - 1, c, eval, map);
	unreach(r, c + 1, eval, map);
	unreach(r, c - 1, eval, map);
}

int	test_unreach(t_game *gm)
{
	t_game	eval;
	char	**map;

	eval = *gm;
	map = duplicate_map(gm);
	if (!map)
		return (0);
	unreach(eval.hero_r, eval.hero_c, eval, map);
	if (eval.exit != 0 || eval.colls != 0)
		return (0);
	return (1);
}
