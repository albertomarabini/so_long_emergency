/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 20:15:54 by prossi            #+#    #+#             */
/*   Updated: 2023/10/29 10:46:29 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "../get_next_line/get_next_line.h"
# include "../minilibx/mlx.h"
# include <errno.h>
# include <limits.h>
# include <stddef.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define ASSET_SIZE 32

typedef struct s_assets
{
	void		*w_tl;
	void		*w_tr;
	void		*w_bl;
	void		*w_br;
	void		*w_l;
	void		*w_r;
	void		*w_t;
	void		*w_b;
	void		*obst;
	void		*coll;
	void		*exit;
	void		*hero;
	void		*floor;
}				t_assets;

typedef struct s_game
{
	char		**map;
	int			map_h;
	int			map_w;
	int			hero;
	int			exit;
	int			colls;
	int			hero_r;
	int			hero_c;
	int			moves;
	void		*mlx;
	void		*win;
	t_assets	imgs;
}				t_game;

void			putstr_fd(const char *s, int fd);
char			*itoa(int n);
char			*xyta(int x, int y);
char			*strdup(const char *s);
int				read_map(t_game *game, int fd);
int				init_mlx(t_game *game);
void			p_img(t_game *game, void *img, int r, int c);
int				err(char *err_1, char *err_2);
int				out(char *s1, char *s2);
int				on_keypress(int key, t_game *game);
int				mlx_destroy_hook(void *win_ptr, t_game *game);
char			*strchr(const char *s, int c);
size_t			strlen(const char *s);
int				free_game(t_game *game);

#endif
