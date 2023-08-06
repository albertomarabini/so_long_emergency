NAME = so_long

CC = gcc
FLAGS = -Wall -Wextra -Werror
LIBS = -lmlx -lXext -lX11

GNL_DIR = get_next_line
SL_DIR = src

GNL_FILES = get_next_line.c get_next_line_utils.c
SL_FILES = so_long.c so_long_utils.c so_long_read_map.c so_long_init_mlx.c so_long_gameplay.c so_long_ft_utils.c

GNL_SRCS = $(addprefix $(GNL_DIR)/, $(GNL_FILES))
SL_SRCS = $(addprefix $(SL_DIR)/, $(SL_FILES))

all: $(NAME)

$(NAME):
	$(CC) $(FLAGS) $(GNL_SRCS) $(SL_SRCS) $(LIBS) -o $(NAME)

clean:
	rm -f $(NAME)

fclean: clean
	rm -f $(NAME)

re: fclean all

norm:
	@norminette $(GNL_SRCS) $(SL_SRCS)

renorm:  fclean all norm

.PHONY: all test clean fclean re
