/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_ft_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:36:11 by amarabin          #+#    #+#             */
/*   Updated: 2023/11/12 20:11:05 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

char	*strdp(const char *s)
{
	char	*dup;
	size_t	len;
	size_t	i;

	len = ft_strlen(s);
	dup = (char *)malloc((len + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	i = 0;
	while (s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

static char	*itoa_to_str(int n, int l)
{
	int		s;
	int		l2;
	char	*str;

	str = (char *)malloc((l + 1) * sizeof(char));
	if (!str)
		return (NULL);
	s = 0;
	l2 = l;
	if (n < 0)
	{
		str[s++] = '-';
		n *= -1;
	}
	while (l > s)
	{
		str[l - 1] = (n % 10) + '0';
		n /= 10;
		l--;
	}
	str[l2] = '\0';
	return (str);
}

char	*itoa(int n)
{
	int	l;
	int	n_copy;

	l = 1;
	n_copy = n;
	if (n == INT_MIN)
		return (strdp("-2147483648"));
	if (n != 0)
	{
		l = 0;
		if (n_copy < 0)
		{
			n_copy *= -1;
			l++;
		}
		while (n_copy > 0)
		{
			n_copy /= 10;
			l++;
		}
	}
	return (itoa_to_str(n, l));
}

char	*xyta(int x, int y)
{
	char	*str;
	char	*str2;
	char	*str3;

	str = itoa(x);
	if (!str)
		return (NULL);
	str2 = ft_strjoin(str, ",");
	free(str);
	if (!str2)
		return (NULL);
	str = itoa(y);
	if (!str)
		return (free(str2), NULL);
	str3 = ft_strjoin(str2, str);
	free(str);
	free(str2);
	if (!str3)
		return (NULL);
	return (str3);
}

void	putstr_fd(const char *s, int fd)
{
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
}
