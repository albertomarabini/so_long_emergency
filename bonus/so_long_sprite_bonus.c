/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_sprite_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 02:45:23 by amarabin          #+#    #+#             */
/*   Updated: 2023/07/29 05:41:06 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

/**
 * Loads the sprite assets into memory using the given path and extension.
 * each frame's file path is composed by
 * path (like "../assets/hero/hero"
 * frame number (from 0 to sprite->len - 1)
 * extension such as ".xpm"
 * Shoud be called after the sprite structure has been initialized.
 */
int	load_sprite_assets(t_sprite *sprite, void *mlx, char *path, char *extension)
{
	char	*filepath;
	char	*str;
	void	*image;
	int		i;

	i = 0;
	while (i <= sprite->len)
	{
		str = ft_strjoin(itoa(i), extension);
		filepath = ft_strjoin(path, str);
		free(str);
		sprite->frames[i] = l_img(mlx, filepath, ASSET_SIZE);
		if (sprite->frames[i] == NULL)
		{
			err(strdup("Unable to load sprite: "), perror(filepath));
			free(filepath);
			return (-1);
		}
		i++;
	}
	return (0);
}

/**
 * Initializes a sprite structure with the specified length and start frame.
 */
t_sprite	*init_sprite(int len, int start)
{
	t_sprite	*sprite;
	int			i;

	sprite = (t_sprite *)malloc(sizeof(t_sprite));
	if (sprite == NULL)
	{
		err(strdup("Unable to instantiate sprite\n"), strerror(errno));
		return (NULL);
	}
	sprite->len = len;
	sprite->stop = 1;
	sprite->current_frame = start;
	sprite->frames = (void **)malloc((len + 1) * sizeof(void *));
	i = 0;
	while (i <= len)
		sprite->frames[i++] = NULL;
	sprite->frames[i] = NULL;
	return (sprite);
}

void	free_sprite(t_sprite *sprite)
{
	if (sprite != NULL)
	{
		i = 0;
		while (i < sprite->len)
		{
			if (sprite->frames[i] != NULL)
				free(sprite->frames[i++]);
		}
		free(sprite->frames);
		free(sprite);
	}
}

/**
 * Retrieves the current frame from the sprite structure.
 * If the 'stop' field of the sprite is 0, the function also increments
 * the current frame position.when it reaches the length of
 * the sprites, the current frame is reset to 0.
 *
 */
void	*get_sprite_frame(t_sprite *sprite)
{
	void	*frame;

	frame = sprite->frames[sprite->current_frame];
	if (!(sprite->stop))
	{
		sprite->current_frame++;
		if (sprite->current_frame == sprite->len)
			sprite->current_frame = 0;
	}
	return (frame);
}
