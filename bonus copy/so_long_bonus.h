/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amarabin <amarabin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 20:15:54 by prossi            #+#    #+#             */
/*   Updated: 2023/08/18 02:11:22 by amarabin         ###   ########.fr       */
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

# define ASSET_SIZE 32
# define DEBUGGER_H 32
# define HERO_TICK 300
# define VILL_TICK 600
# define GO_TICK 4000
# define MSG_TICK 2000
# define TGT_Q_LEN 3
# define CORNER_COUNT 4
# define ASTS_PATH "/root/C/so_long/assets/"
# define V_ASTS_PATH "/root/C/so_long/assets/vill/"
# define H_ASTS_PATH "/root/C/so_long/assets/hero/"
# define W_NAME "So long Goomba"

typedef struct s_point
{
	int					r;
	int					c;
	int					val;
	int					val2;
}						t_point;

typedef struct s_sel_tgt_obj_mechanics
{
	t_point				**(*sort)(t_point **, int);
	int					best;
}						t_sel_tgt_obj_mechanics;

typedef struct s_sprite
{
	int					len;
	int					stop;
	int					current_frame;
	void				**frames;
	char				*path;
	char				*ext;
}						t_stp;

typedef struct s_pair
{
	t_point				a;
	t_point				b;
}						t_pair;

typedef struct s_assets
{
	void				*w_tl;
	void				*w_tr;
	void				*w_bl;
	void				*w_br;
	void				*w_l;
	void				*w_r;
	void				*w_t;
	void				*w_b;
	void				*obst;
	void				*coll;
	void				*exit;
	void				*hero;
	void				*floor;
	void				*vill;
	void				*deb;
}						t_assets;

typedef struct s_game
{
	char				**map;
	char				**o_map;
	int					map_h;
	int					map_w;
	int					hero;
	int					exits;
	int					colls;
	int					tgt_el_q[TGT_Q_LEN];
	int					vills;
	int					hero_r;
	int					hero_c;
	int					moves;
	int					run;
	int					run_unsafe;
	t_point				run_to;
	void				*mlx;
	void				*win;
	struct timespec		*hero_clock;
	struct timespec		*vill_clock;
	struct timespec		*msg_clock;
	t_point				**a_vills;
	t_point				**a_colls;
	t_point				**a_exits;
	t_point				**a_corners;
	t_assets			i;
	t_stp				*hero_spt;
	t_stp				**vill_spt;
	int					is_on;
	char				*msg_out;
	int					***v_paths_m;
	int					***c_paths_m;
	t_point				**c_points;
}						t_game;

/* /ft/ft_utils1_bonus.c */
char					*c_strerror(void);
char					*strdup(const char *s);
char					*itoa(int n);
char					*xyta(int x, int y);
/* /ft/ft_utils2_bonus.c */
void					putstr_fd(const char *s, int fd);
int						trn(int cond, int if_true, int if_false);
int						abs(int val);
int						within(int a, int b, int bound);
void					*ft_memcpy(void *dest, const void *src, size_t n);
/* /utils/utils1_bonus.c */
void					*c_alloc(t_game *game, size_t size);
long long				elapsed_time(struct timespec *s, t_game *game);
t_pair					new_ppair(t_point a, t_point b);
char					**duplicate_map(t_game *gm);
int						inst_tgt_path_structs(int n, int ****c_paths_m,
							t_point ****c_points);
/* /utils/utils2_bonus.c */
t_game					*create_safe_game(t_game *gm, int safe_w);
int						**init_path_matrix(t_game *game);
t_sel_tgt_obj_mechanics	sto_mech(t_point **(*sort)(t_point **, int), int best);
/* /utils/utils3_bonus.c */
int						err(char *err_1, char *err_2);
int						out(char *s1, char *s2, t_game *game);
/* /utils/p_utils_bonus.c */
t_point					duplicate_p(t_point p);
t_point					*create_pp(int r, int c, int val, int val2);
t_point					*p_to_pp(t_point p);
/* /utils/p_utils2_bonus.c */
t_point					create_p(int r, int c, int val, int val2);
t_point					null_p(void);
int						is_null_p(t_point p);
/* /utils/mlx_utils_bonus.c */
void					*l_img(void *mlx, char *path, char *img, int s);
void					p_img(t_game *game, void *img, int r, int c);
int						intantiate_mlx_window(t_game *gm, char *name);
int						mlx_destroy_hook(void *mlx_ptr, void *win_ptr);
/* /ft/ap_utils_bonus.c */
t_point					**rev_sort_ap(t_point **arr, int n);
void					free_ap(t_point **arr);
int						len_ap(t_point **p);
/* /ft/ap_utils2_bonus.c */
t_point					**sort_ap(t_point **arr, int n);
t_point					**rev_ap(t_point **arr, int n);
/* /ft/a_utils_bonus.c*/
void					a_shift(int *a, int head, int length);
void					set_a(int *a, int length, int value);
void					null_a(int *a, int length);
int						is_null_a(int *a, int length);
int						a_contains(int *a, int val, int length);
/* /utils/read_map_bonus.c */
int						read_map(t_game *game, int fd);

/* /utils/read_map2_bonus.c */
void					populate_tgt_arrays(t_game *gm);
int						validate_map_content(t_game *game);
t_point					**find_corners(t_game *gm);
/* /so_long_gameplay_bonus.c */
int						on_keypress(int key, t_game *game);
/* /so_long_init_mlx_bonus.c */
int						init_mlx(t_game *game);
/* /so_long_gameplay_bonus.c */
int						render_next_frame(t_game *game);
/* /so_long_bonus.c */
void					free_map(char **map, int h);
void					free_game(t_game *game);
void					free_image(void *mlx, void *img);
/* /so_long_sprite_bonus.c */
void					*get_sprite_frame(t_stp *sprite);
void					free_sprite_a(t_game *gm, t_stp **sprites);
void					free_sprite(t_game *gm, t_stp *sprite);
t_stp					*init_sprite(t_game *game, t_point start_len,
							char *path, char *ext);
/* /game_logic/find_hero_move_bonus.c */
t_point					find_hero_move(t_game *gm);
/* /game_logic/find_shortest_path_bonus.c */
t_point					find_shortest_path(t_point start, t_point end,
							t_game *game, int tb);
t_point					find_distance(t_point start, t_point end, t_game *game);
int						will_it_crash(t_point a, t_point b, t_point t,
							t_game *gm);
/* /game_logic/sel_tgt_obj_bonus.c */
void					sel_tgt(int ***pts, t_point p, int (*q)[TGT_Q_LEN],
							t_sel_tgt_obj_mechanics m);
t_point					is_path_to_tgt_nsafe(t_point t, t_game *gm,
							int ***v_paths_m, char *pattern_c);
/* /game_logic/collect_paths_bonus.c */
int						***collect_corn_paths(t_game *gm, t_point ***c_points,
							char *pattern_c);
int						collect_tgt_paths(t_game *gm, char **pattern_c);
int						***collect_vill_paths(t_game *game);
/* /game_logic/lay_paths_bonus.c */
t_point					trak_back_paths(t_point t, int **paths, t_game *game,
							int tb);
void					lay_paths(t_pair p, int **pth, t_game *gm, char *ptrn);
/* /game_logic/freers_bonus.c */
void					free_path(int **paths);
void					free_paths_matrix(int ***paths);
void					free_safe_map(t_game *game);
void					free_tgt_path_structs(int ***c_paths_m,
							t_point **c_points);

#endif
