NAME = gc

CC = cc

CFLAGS = 

SRCS = gc.c gc_utils.c gc_malloc.c gc_track.c gc_unreachable.c
OBJS = $(SRCS:.c=.o)


all: $(NAME)
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -fsanitize=address -o $(NAME) $(OBJS)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)