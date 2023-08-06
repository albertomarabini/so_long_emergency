/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 09:20:21 by amarabin          #+#    #+#             */
/*   Updated: 2023/07/05 18:52:47 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/**
 * Deallocates or allocates memory (or both) for a file buffer.
 *
 * @param fd The file descriptor associated with the buffer.
 * @param allocate Flag indicating whether to allocate memory for the buffer (1).
 * @param old_buffer Pointer to the pointer of the old buffer to deallocate.
 *                   If NULL will jump to evaluate allocate flag.
 * @return A pointer to the allocated buffer on success, or NULL on failure or
 *         deallocation.
 */
t_file_info	*buffer_allocation(int fd, int allocate, t_file_info **old_buffer)
{
	t_file_info	*buffer;

	buffer = NULL;
	if (old_buffer && *old_buffer)
	{
		if ((*old_buffer)->content)
			free((*old_buffer)->content);
		free(*old_buffer);
		*old_buffer = NULL;
	}
	if (allocate)
	{
		buffer = (t_file_info *)malloc(sizeof(t_file_info));
		if (!buffer)
			return (NULL);
		buffer->fd = fd;
		buffer->content = NULL;
		buffer->cursor = NULL;
		buffer->eof = 0;
	}
	return (buffer);
}

static char	*extract_line(t_file_info *buffer)
{
	int		i;
	char	*line;

	i = 0;
	while (buffer->cursor[i] && buffer->cursor[i] != '\n')
		i++;
	line = (char *)malloc((i + 1) * sizeof(char));
	if (!line)
		return (NULL);
	line[i] = '\0';
	i = 0;
	while (buffer->cursor[i] && buffer->cursor[i] != '\n')
	{
		line[i] = buffer->cursor[i];
		i++;
	}
	if (buffer->cursor[i] == '\n')
		buffer->cursor += i + 1;
	else if (buffer->eof)
	{
		free(buffer->content);
		buffer->content = NULL;
	}
	return (line);
}

/**
 * man read
 *
 * On success, 1 is returned (zero indicates
 * end of file), and the file position is advanced by this number.
 * It is not an error if this number is smaller than the number of
 * bytes requested; this may happen for example because fewer bytes
 * are actually available right now (maybe because we were close to
 * end-of-file, or because we are reading from a pipe, or from a
 * terminal), or because read() was interrupted by a signal. See
 * also NOTES.
 *
 * On error, -1 is returned, and errno is set to indicate the error.
 * In this case, it is left unspecified whether the file position
 * (if any) changes.
 */

static int	read_fd(int fd, t_file_info *buffer)
{
	int		n_bytes;
	char	*tmp;

	n_bytes = BUFFER_SIZE;
	tmp = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!tmp)
		return (0);
	while (!ft_strchr(buffer->cursor, '\n') && !buffer->eof)
	{
		n_bytes = read(fd, tmp, BUFFER_SIZE);
		if (n_bytes == -1)
		{
			free(tmp);
			return (0);
		}
		if (n_bytes != BUFFER_SIZE)
			buffer->eof = 1;
		tmp[n_bytes] = '\0';
		buffer->cursor = ft_strjoin(buffer->cursor, tmp);
		if (buffer->content)
			free(buffer->content);
		buffer->content = buffer->cursor;
	}
	free(tmp);
	return (1);
}

/**
 * Reads one line at a time from a file descriptor.
 *
 * @param fd    The file descriptor to read from.
 * @param line  A pointer to a char pointer that will be updated with the
 *              address of the line read. The memory for the line will be
 *              allocated by the function and should be freed by the caller.
 *
 * @return      1 if a line was successfully read.
 *              0 if the end of the file has been reached.
 *              -1 if an error occurred.
 *
 * Successive calls to this function will allow reading the entire text
 * available on the file descriptor, one line at a time, up to the end
 * of the file. The function handles reading from both file descriptors
 * and standard input. The buffer size is defined at compile time using
 * the BUFFER_SIZE param. We assume the stream will stay open until the
 * the function is being used. Once reached the end of the file, the
 * function will deallocate the buffer and return 0.
 */
int	get_next_line(int fd, char **line)
{
	static t_file_info	*buffer;

	if (!line || fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > SSIZE_MAX)
		return (-1);
	*line = NULL;
	if (!buffer || buffer->fd != fd)
		buffer = buffer_allocation(fd, 1, &buffer);
	if (!buffer)
		return (-1);
	if (!buffer->eof)
	{
		if (!read_fd(fd, buffer))
		{
			buffer_allocation(fd, 0, &buffer);
			return (-1);
		}
	}
	*line = extract_line(buffer);
	if (!buffer->content)
	{
		buffer_allocation(fd, 0, &buffer);
		return (0);
	}
	return (1);
}
