#include "gc.h"
#include <string.h>

void	gc_scan_area(t_gc *gc, unsigned long *start, unsigned long *end)
{
	void	*maybe_ptr;

	while (start < end)
	{
		maybe_ptr = (void *)*start;
		gc_mark_ptr_if_found(gc, maybe_ptr);
		start++;
	}
}

static void gc_mark_stack_and_root(t_gc *gc, void *bot_stack)
{
    unsigned long local_var;
    unsigned long *stack_pointer;
    t_gc_root *r;

    stack_pointer = (unsigned long *)&local_var;
    gc_scan_area(gc, stack_pointer, (unsigned long *)bot_stack);
    r = gc->roots;
    while (r)
    {
        gc_scan_area(gc, (unsigned long *)r->start, (unsigned long *)r->end);
        r = r->next;
    }
}

static void	gc_sweep_unmarked(t_gc *gc)
{
	t_gc_node *cur;
	t_gc_node *prev;
	t_gc_node *next;

	cur = gc->head;
	prev = NULL;
	while (cur)
	{
		next = cur->next; 
		if (cur->mark == 0)
		{
			if (prev)
				prev->next = next;
			else
				gc->head = next;
			free(cur->ptr);
			free(cur);
		}
		else
			prev = cur;
		cur = next;
	}
}


void	gc_collect_unreachable(t_gc *gc, void *stack_bottom)
{
	if (gc->paused)
		return ;
	gc_reset_mark(gc);
	gc_mark_stack_and_root(gc, stack_bottom);
	gc_sweep_unmarked(gc);
}
