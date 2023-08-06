/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 20:15:54 by prossi            #+#    #+#             */
/*   Updated: 2023/08/03 22:59:46 by amarabin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_BONUS_H
# define SO_LONG_BONUS_H

# include "../get_next_line/get_next_line.h"
# include "../minilibx/mlx.h"
# include "../minilibx/mlx_int.h"
# include <errno.h>
# include <limits.h>
# include <stddef.h>
# include <stdlib.h>
# include <time.h>
# include <unistd.h>

# define ASSET_SIZE 48
# define DEBUGGER_H 48

typedef struct s_point
{
	int				r;
	int				c;
	int				val;
}					t_point;

typedef struct s_sprite
{
	int				len;
	int				stop;
	int				current_frame;
	void			**frames;

}					t_sprite;

typedef struct s_assets
{
	void			*w_tl;
	void			*w_tr;
	void			*w_bl;
	void			*w_br;
	void			*w_l;
	void			*w_r;
	void			*w_t;
	void			*w_b;
	void			*obst;
	void			*coll;
	void			*exit;
	void			*hero;
	void			*floor;
	void			*vill;
	void			*deb;
}					t_assets;

typedef struct s_game
{
	char			**map;
	int				map_h;
	int				map_w;
	int				hero;
	int				exit;
	int				colls;
	int				vills;
	int				hero_r;
	int				hero_c;
	int				moves;
	void			*mlx;
	void			*win;
	struct timespec	*start;
	t_point			**a_vills;
	t_assets		imgs;
}					t_game;

void				putstr_fd(const char *s, int fd);
char				*itoa(int n);
char				*xyta(int x, int y);
char				*strdup(const char *s);
int					read_map(t_game *game, int fd);
int					init_mlx(t_game *game);
void				p_img(t_game *game, void *img, int r, int c);
int					err(char *err_1, char *err_2);
int					out(char *s1, char *s2, t_game *game);
int					on_keypress(int key, t_game *game);
int					mlx_destroy_hook(void *mlx_ptr, void *win_ptr);
t_point				*create_point(int r, int c, int val);
int					render_next_frame(t_game *game);
long long			elapsed_time(struct timespec *s);
t_point				*find_shortest_path(t_point *start, t_point *end,
						t_game *game);

#endif
