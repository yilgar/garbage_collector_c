#ifndef GC_H
# define GC_H


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdint.h>


typedef struct s_gc_node
{
	int mark;
	void *ptr;
	size_t size;
	struct s_gc_node *next;
} t_gc_node;

typedef struct s_gc_root
{
	void	*end;
	void	*start;
	struct  s_gc_root	*next;
} t_gc_root;


typedef struct s_gc
{
	t_gc_node *head;
	t_gc_root *roots;

	int		paused;
} t_gc;

//gc utils
void	*gc_malloc(t_gc *gc, size_t size);
// void	*gc_realloc(t_gc *gc, void *old_ptr, size_t new_size); lazım olmayabilir
void	gc_unregister(t_gc *gc, void *ptr);
void	*gc_calloc(t_gc *gc, size_t count, size_t size);
void	gc_register(t_gc *gc, void *context, size_t size);
void	gc_clear(t_gc *gc);
void	gc_free_one(t_gc *gc, void *ptr);

//gc reachable utils
void	gc_track_roots(t_gc *gc, void *start, void *end);
void gc_reset_mark(t_gc *gc);
void	gc_mark_ptr_if_found(t_gc *gc, void *possible);
void	gc_collect_unreachable(t_gc *gc, void *stack_bottom);

//gc funcs
char	*gc_strdup(t_gc *gc, const char *s);
char	*gc_strjoin(t_gc *gc, char const *s1, char const *s2);
void	gc_pause(t_gc *gc);
void	gc_resume(t_gc *gc);
char	**gc_split(t_gc *gc, const char *s, char c);


#endif

