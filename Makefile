NAME = gc

NAME_FORK = gc_fork

CC = cc

CFLAGS = 

SRCS = gc.c gc_utils.c gc_malloc.c gc_track.c gc_unreachable.c gc_split.c


SRCS_FORK = gc_fork.c gc_utils.c gc_malloc.c gc_track.c gc_unreachable.c gc_split.c

OBJS = $(SRCS:.c=.o)


OBJS_FORK = $(SRCS_FORK:.c=.o)


all: $(NAME)
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -g -o $(NAME) $(OBJS)


fork: $(NAME_FORK)
$(NAME_FORK): $(OBJS_FORK)
	@$(CC) $(CFLAGS) -o $(NAME_FORK) $(OBJS_FORK)

clean:
	rm -rf $(OBJS) $(OBJS_FORK)

fclean: clean
	rm -rf $(NAME) $(NAME_FORK)

re: fclean all

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./gc


asan: fclean fork
	@echo "fork asan"
	@$(CC) $(CFLAGS) -fsanitize=address -o $(NAME_FORK) $(OBJS_FORK)


.PHONY: valgrind all clean fclean re 