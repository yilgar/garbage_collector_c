#include "gc.h"

static t_gc_root	*new_root(void *start, void *end)
{
	t_gc_root	*r;

	r = NULL;
	r = malloc(sizeof(t_gc_root));
	if (!r)
		return (NULL);
	r->start = start;
	r->end = end;
	r->next = NULL;
	return (r);
}

// void	gc_mark_ptr_if_found(t_gc *gc, void *possible)
// {
// 	unsigned long	addr;
// 	t_gc_node	*cur;

// 	addr = (unsigned long)possible;

// 	//linux safe mem alanı
// 	if (addr < 4096 || addr > 0x7fffffffffff) 
// 		return ;
// 	cur = gc->head;
// 	while (cur)
// 	{
// 		if (cur->ptr == possible && cur->mark == 0)
// 		{
// 			cur->mark = 1;
// 			break;
// 		}
// 		cur = cur->next;
// 	}
// }

// void	gc_mark_ptr_if_found(t_gc *gc, void *possible)
// {
// 	unsigned long	addr;
// 	t_gc_node		*cur;

// 	if (!possible)
// 		return ;
// 	addr = (unsigned long)possible;

// 	// hem adres aralığı hem hizalama kontrolü
// 	if (addr < 4096 || addr > 0x7fffffffffff || (addr % sizeof(void *)) != 0)
// 		return;

// 	cur = gc->head;
// 	while (cur)
// 	{
// 		if (cur->ptr == possible && cur->mark == 0)
// 		{
// 			cur->mark = 1;
// 			break;
// 		}
// 		cur = cur->next;
// 	}
// }

// void	gc_mark_ptr_if_found(t_gc *gc, void *possible)
// {
// 	unsigned long	addr;
// 	t_gc_node		*cur;

// 	if (!possible)
// 		return ;

// 	addr = (unsigned long)possible;
// 	if (addr < 4096 || addr > 0x7fffffffffff || (addr % sizeof(void *)) != 0)
// 		return;

// 	cur = gc->head;
// 	while (cur)
// 	{
// 		if (cur->ptr == possible)
// 		{
// 			cur->mark = 1;
// 			break;
// 		}
// 		cur = cur->next;
// 	}
// }

void	gc_mark_ptr_if_found(t_gc *gc, void *possible)
{
	if (!possible)
		return;

	unsigned long addr = (unsigned long)possible;

	if (addr < 4096 || addr > 0x7fffffffffff || (addr % sizeof(void *) != 0))
		return;

	t_gc_node *cur = gc->head;
	while (cur)
	{
		if (cur->ptr == possible)
		{
			printf("✅ MARKED ptr: %p (size %lu)\n", cur->ptr, cur->size);
			cur->mark = 1;
			break;
		}
		cur = cur->next;
	}
}




void gc_reset_mark(t_gc *gc)
{
	t_gc_node *cur;

	cur = gc->head;
	while (cur)
	{
		cur->mark = 0;
		cur = cur->next;
	}
}

void	gc_track_roots(t_gc *gc, void *start, void *end)
{
	t_gc_root	*root;

	root = new_root(start, end);
	if (!root)
		return ;
	root->next = gc->roots;
	gc->roots = root;
}
