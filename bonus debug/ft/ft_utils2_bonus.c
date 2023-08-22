/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils2_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:04:07 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/14 19:02:54 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../so_long_bonus.h"

void	putstr_fd(const char *s, int fd)
{
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
}

int	trn(int cond, int if_true, int if_false)
{
	if (cond)
		return (if_true);
	return (if_false);
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
int	within(int a, int b, int bound)
{
	return (abs(a - b) <= bound);
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
