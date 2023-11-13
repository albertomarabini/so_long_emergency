/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_sprite_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 02:45:23 by amarabin          #+#    #+#             */
/*   Updated: 2023/11/12 19:49:05 by amarabin         ###   ########.fr       */
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
static int	load_sprite_assets(t_stp *sprite, void *mlx)
{
	char	*name;
	char	*tmp;
	int		i;

	i = 0;
	sprite->frames[0] = NULL;
	while (i < sprite->len)
	{
		tmp = itoa(i);
		name = ft_strjoin(tmp, sprite->ext);
		if (!tmp || !name)
			return (err(strdp("Unable to load sprite: "), c_strerror()));
		free(tmp);
		sprite->frames[i + 1] = NULL;
		sprite->frames[i] = l_img(mlx, sprite->path, name, ASSET_SIZE);
		if (sprite->frames[i] == NULL)
		{
			free(name);
			return (err(strdp("Unable to load sprite: "), c_strerror()));
		}
		free(name);
		i++;
	}
	return (1);
}

/**
 * Initializes a sprite structure with the specified length and start frame.
 */
t_stp	*init_sprite(t_game *game, t_point start_len, char *path, char *ext)
{
	t_stp	*sprite;

	if (path == NULL || ext == NULL)
		c_throw(game, NULL, NULL);
	sprite = (t_stp *)c_alloc(game, sizeof(t_stp));
	sprite->len = start_len.c;
	sprite->stop = 0;
	sprite->current_frame = start_len.r;
	sprite->path = path;
	sprite->ext = ext;
	sprite->frames = (void **)c_alloc(game, (sprite->len + 1) * sizeof(void *));
	if (!sprite->frames || load_sprite_assets(sprite, game->mlx) == -1)
	{
		free_sprite(game, sprite);
		return (NULL);
	}
	return (sprite);
}

void	free_sprite(t_game *gm, t_stp *sprite)
{
	int	i;

	i = 0;
	if (sprite != NULL)
	{
		i = 0;
		while (sprite->len > i++)
		{
			if (sprite->frames[i - 1] != NULL)
				free_image(gm->mlx, sprite->frames[i - 1]);
		}
		free(sprite->frames);
		free(sprite->path);
		free(sprite->ext);
		free(sprite);
	}
}

void	free_sprite_a(t_game *gm, t_stp **sprites)
{
	int	i;

	i = 0;
	while (sprites[i] != NULL)
		free_sprite(gm, sprites[i++]);
	free(sprites);
}

/**
 * Retrieves the current frame of the sprite.
 * If the 'stop' field of the sprite is 0, the function also increments
 * the current frame position.when it reaches the length of
 * the sprites, the current frame is reset to 0.
 *
 */
void	*get_sprite_frame(t_stp *sprite)
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
