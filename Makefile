NAME = so_long
BONUS_NAME = $(NAME)_bonus

CC = gcc
FLAGS = -Wall -Wextra -Werror
LIBS = -lmlx -framework OpenGL -framework AppKit

GNL_DIR = get_next_line
SL_DIR = src
BONUS_FT_DIR = ft
BONUS_GAME_LOGIC_DIR = game_logic
BONUS_UTILS_DIR = utils
BONUS_MAIN_DIR = bonus

LIBGNL = libgnl.a

GNL_FILES = get_next_line.c get_next_line_utils.c
SL_FILES = so_long.c so_long_utils.c so_long_read_map.c so_long_init_mlx.c so_long_gameplay.c so_long_ft_utils.c
SL_BONUS_FILES_UTILS = utils1_bonus.c utils2_bonus.c utils3_bonus.c utils4_bonus.c utils5_bonus.c read_map_bonus.c read_map2_bonus.c mlx_utils_bonus.c
SL_BONUS_FILES_GAME_LOGIC = find_hero_move_bonus.c freers_bonus.c find_shortest_path_bonus.c lay_paths_bonus.c collect_paths_bonus.c sel_tgt_obj_bonus.c
SL_BONUS_FILES_FT = ft_utils2_bonus.c ft_utils1_bonus.c p_utils_bonus.c p_utils2_bonus.c a_utils_bonus.c ap_utils_bonus.c ap_utils2_bonus.c
SL_BONUS_FILES_MAIN = so_long_bonus.c so_long_init_mlx_bonus.c so_long_gameplay_bonus.c so_long_gameplay2_bonus.c so_long_sprite_bonus.c

GNL_SRCS = $(addprefix $(GNL_DIR)/, $(GNL_FILES))
SL_SRCS = $(addprefix $(SL_DIR)/, $(SL_FILES))
SL_BONUS_SRCS_FT = $(addprefix $(BONUS_MAIN_DIR)/$(BONUS_FT_DIR)/, $(SL_BONUS_FILES_FT))
SL_BONUS_SRCS_GAME_LOGIC = $(addprefix $(BONUS_MAIN_DIR)/$(BONUS_GAME_LOGIC_DIR)/, $(SL_BONUS_FILES_GAME_LOGIC))
SL_BONUS_SRCS_UTILS = $(addprefix $(BONUS_MAIN_DIR)/$(BONUS_UTILS_DIR)/, $(SL_BONUS_FILES_UTILS))
SL_BONUS_SRCS_MAIN = $(addprefix $(BONUS_MAIN_DIR)/, $(SL_BONUS_FILES_MAIN))

all: mlx $(NAME)

bonus: mlx $(BONUS_NAME)

mlx:
	make -C ./minilibx

$(LIBGNL): $(GNL_SRCS)
	$(CC) $(FLAGS) -c $^
	ar rcs $@ get_next_line.o get_next_line_utils.o

$(NAME): $(LIBGNL) $(SL_SRCS)
	$(CC) $(FLAGS) $(SL_SRCS) -L. -lgnl -L./minilibx $(LIBS) -g -o $(NAME)

$(BONUS_NAME): $(LIBGNL) $(SL_BONUS_SRCS_FT) $(SL_BONUS_SRCS_GAME_LOGIC) $(SL_BONUS_SRCS_UTILS) $(SL_BONUS_SRCS_MAIN) 
	$(CC) $(FLAGS) $(SL_BONUS_SRCS_FT) $(SL_BONUS_SRCS_GAME_LOGIC) $(SL_BONUS_SRCS_UTILS) $(SL_BONUS_SRCS_MAIN) -L. -lgnl -L./minilibx $(LIBS) -g -o $(BONUS_NAME)

debug: $(LIBGNL) $(SL_SRCS)
	$(CC) $(FLAGS) -g $(SL_SRCS) -L. -lgnl -L./minilibx $(LIBS) -o ../debugger/a.out

debug-bonus: mlx $(LIBGNL)
	$(CC) $(FLAGS) -g $(SL_BONUS_SRCS_FT) $(SL_BONUS_SRCS_GAME_LOGIC) $(SL_BONUS_SRCS_UTILS) $(SL_BONUS_SRCS_MAIN) -L. -lgnl $(LIBS) -o ../debugger/a.out

clean:
	make -C ./minilibx clean
	rm -f get_next_line.o get_next_line_utils.o $(BONUS_NAME) $(LIBGNL)

fclean: clean
	rm -f $(LIBGNL) $(NAME) $(BONUS_NAME)

re: fclean all

norm:
	@norminette $(GNL_SRCS) $(SL_SRCS) $(SL_BONUS_SRCS_FT) $(SL_BONUS_SRCS_GAME_LOGIC) $(SL_BONUS_SRCS_UTILS) $(SL_BONUS_SRCS_MAIN)

renorm:  fclean all norm

.PHONY: all bonus clean fclean re norm renorm

