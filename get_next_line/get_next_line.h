/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 09:20:32 by amarabin          #+#    #+#             */
/*   Updated: 2023/07/05 18:52:53 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <fcntl.h>
# include <limits.h>
# include <stdlib.h>
# include <sys/resource.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 2000
# endif

typedef struct s_file_info
{
	int		fd;
	char	*content;
	char	*cursor;
	int		eof;
}			t_file_info;

char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strchr(const char *s, int c);
size_t		ft_strlen(const char *s);
int			get_next_line(int fd, char **line);

#endif
