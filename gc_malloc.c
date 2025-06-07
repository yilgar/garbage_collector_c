#include "gc.h"

static t_gc_node *new_node(void *ptr, size_t size)
{
	t_gc_node *node = malloc(sizeof(t_gc_node));
	if (!node)
		return (NULL);
	node->ptr = ptr;
	node->size = size;
	node->mark = 0;
	node->next = NULL;
	return (node);
}

void	*gc_malloc(t_gc *gc, size_t size)
{
	void *ptr = malloc(size);
	if (!ptr)
		return (NULL);
	gc_register(gc, ptr, size);
	return (ptr);
}


void	*gc_calloc(t_gc *gc, size_t count, size_t size)
{
	void	*ptr;

	ptr = calloc(count, size);
	if (!ptr)
		return (NULL);
	gc_register(gc, ptr, size);
	return (ptr);
}

void	gc_register(t_gc *gc, void *ptr, size_t size)
{
	t_gc_node *node = new_node(ptr, size);
	if (!node)
		return ;
	node->next = gc->head;
	gc->head = node;
}


void	gc_unregister(t_gc *gc, void *ptr)
{
	t_gc_node	*cur;
	t_gc_node	*prev;

	cur = gc->head;
	prev = NULL;
	while (cur)
	{
		if (cur->ptr == ptr)
		{
			if (prev)
				prev->next = cur->next;
			else
				gc->head = cur->next;
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}
 

/* lazım olmayabilir duruma göre v0.1*/
// void	*gc_realloc(t_gc *gc, void *old_ptr, size_t new_size)
// {
// 	void	*new_ptr;

// 	new_ptr = realloc(old_ptr, new_size);
// 	if (!new_ptr)
// 		return (NULL);
// 	if (new_ptr != old_ptr)
// 	{
// 		gc_register(gc, new_ptr);
// 		gc_unregister(gc, old_ptr);
// 	}
// 	return (new_ptr);
// }

void	gc_free_one(t_gc *gc, void *ptr)
{
	t_gc_node	*cur;
	t_gc_node	*prev;

	cur = gc->head;
	prev = NULL;
	while (cur)
	{
		if (cur->ptr == ptr)
		{
			if (prev)
				prev->next = cur->next;
			else
				gc->head = cur->next;
			free(cur->ptr);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

void	gc_clear(t_gc *gc)
{
	t_gc_node	*tmp_node;
	t_gc_root	*tmp_root;

	while (gc->head)
	{
		tmp_node = gc->head->next;
		free(gc->head->ptr);
		free(gc->head);
		gc->head = tmp_node;
	}
	// GC root'larını temizle
	while (gc->roots)
	{
		tmp_root = gc->roots->next;
		free(gc->roots);
		gc->roots = tmp_root;
	}
}
