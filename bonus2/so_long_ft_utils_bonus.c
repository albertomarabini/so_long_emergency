/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_ft_utils_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:36:11 by amarabin          #+#    #+#             */
/*   Updated: 2023/08/08 13:53:24 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

void	*c_alloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		printf("Error\nMemory allocation failed\n");
		//exit(1);
	}
	//printf("%zu byte(s) allocated\n", size);
	return (ptr);
}
void c_frr(char *reason, void *ptr)
{
	//printf("memory deallocated for %s\n", reason);
	free(ptr);
}

char	*strdup(const char *s)
{
	char	*dup;
	size_t	len;
	size_t	i;

	len = ft_strlen(s);
	dup = (char *)c_alloc((len + 1) * sizeof(char));
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

	str = (char *)c_alloc((l + 1) * sizeof(char));
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
		return (strdup("-2147483648"));
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
	str2 = ft_strjoin(str, ",");
	c_frr("", str);
	str = itoa(y);
	str3 = ft_strjoin(str2, str);
	c_frr("", str);
	c_frr("", str2);
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

/**
 * @brief Get the elapsed time in milliseconds
 *
 * @param start
 * @return long long
 */
long long	elapsed_time(struct timespec *s)
{
	long long		elapsed;
	struct timespec	*e;

	e = (struct timespec *)c_alloc(sizeof(struct timespec));
	clock_gettime(CLOCK_MONOTONIC, e);
	elapsed = (e->tv_sec - s->tv_sec) * 1000 + (e->tv_nsec - s->tv_nsec) / 1e6;
	c_frr("", e);
	return (elapsed);
}

/**
 * creates a new point with the given row, column and value.
 */
t_point	*create_p(int r, int c, int val)
{
	t_point	*point;

	point = (t_point *)c_alloc(sizeof(t_point));
	if (!point)
		return (NULL);
	point->r = r;
	point->c = c;
	point->val = val;
	point->val2 = 0;
	return (point);
}

/**
 * creates a new point with the given row, column and value.
 */
t_point	create_sp(int r, int c, int val)
{
	t_point	point;

	point.r = r;
	point.c = c;
	point.val = val;
	point.val2 = 0;
	return (point);
}

/**
 * creates a new point with the given row, column and value.
 */
t_point	*create_p2(int r, int c, int val, int val2)
{
	t_point	*point;
	//rename thius thing!!!
	point = (t_point *)c_alloc(sizeof(t_point));
	if (!point)
		return (NULL);
	point->r = r;
	point->c = c;
	point->val = val;
	point->val2 = val2;
	return (point);
}

/**
 * creates a new point with the given row, column and value.
 */
t_point	create_sp2(int r, int c, int val, int val2)
{
	t_point	point;

	point.r = r;
	point.c = c;
	point.val = val;
	point.val2 = val2;
	return (point);
}

t_point null_p(void)
{
	t_point	point;

	point.r = INT_MAX;
	point.c = INT_MAX;
	point.val = INT_MAX;
	point.val2 = INT_MAX;
	return (point);
}

int is_null_p(t_point p)
{
	if (p.r == INT_MAX && p.c == INT_MAX && p.val == INT_MAX && p.val2 == INT_MAX)
		return (1);
	return (0);
}

/**
 * sorts an array of p ascending. Returns the value to save some space
 * somewhere else
 */
t_point **sort_p(t_point **arr, int n)
{
	int		i;
	int		j;
	t_point	*temp;

	i = 0;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - i - 1)
		{
			if (arr[j]->val > arr[j + 1]->val)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
			else if (arr[j]->val == arr[j + 1]->val)
			{
				if (arr[j]->val2 > arr[j + 1]->val2)
				{
					temp = arr[j];
					arr[j] = arr[j + 1];
					arr[j + 1] = temp;
				}
			}
			j++;
		}
		i++;
	}
	return (arr);
}

void	rev_p(t_point **arr, int n)
{
	int	i;
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
}

void	free_p(t_point **arr)
{
	int	i;

	if(!arr)
		return ;
	i = 0;
	while (arr[i] != NULL)
		c_frr("", arr[i++]);
	c_frr("", arr);
}

int	trn(int cond, int if_true, int if_false)
{
	if (cond)
		return (if_true);
	return (if_false);
}
