#include "gc.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

// int main(void)
// {
// 	t_gc parent_gc;
// 	t_gc child_gc;
// 	pid_t pid;

// 	parent_gc.head = NULL;
// 	parent_gc.roots = NULL;
// 	parent_gc.paused = 0;

// 	char *parent_kept = gc_strdup(&parent_gc, "parent hayatta kalacak");
// 	char *parent_gone = gc_strdup(&parent_gc, "parent silinecek");

// 	gc_track_roots(&parent_gc, &parent_kept, (void *)&parent_kept + sizeof(char *));

// 	pid = fork();
// 	if (pid < 0)
// 	{
// 		perror("fork");
// 		return (1);
// 	}
// 	else if (pid == 0)
// 	{
// 		// === CHILD ===
// 		child_gc.head = NULL;
// 		child_gc.roots = NULL;
// 		child_gc.paused = 0;

// 		char *child_keep = gc_strdup(&child_gc, "çocuk kurtulacak");
// 		char *child_lost = gc_strdup(&child_gc, "çocuk gidecek");

// 		// sadece child_keep'i track 
// 		gc_track_roots(&child_gc, &child_keep, (void *)&child_keep + sizeof(char *));

// 		child_lost = NULL;

// 		printf("\nchild process\n");
// 		gc_collect_unreachable(&child_gc, __builtin_frame_address(0));

// 		if (child_keep)
// 			printf("child kept: %s\n", child_keep);
// 		if (child_lost)
// 			printf("child lost: %s should freed??????\n", child_lost);
// 		else
// 			printf("child lost is freed\n");

// 		gc_clear(&child_gc);
// 	}
// 	else
// 	{
// 		printf("\n parent process\n");
// 		parent_gone = NULL;

// 		gc_collect_unreachable(&parent_gc, __builtin_frame_address(0));

// 		if (parent_kept)
// 			printf("parent kept: %s\n", parent_kept);
// 		if (parent_gone)
// 			printf(" parent gone %s should freed\n", parent_gone);
// 		else
// 			printf(" parent is freed\n");

// 		gc_clear(&parent_gc);
// 	}

// 	return (0);
// }

int main(void)
{
	t_gc gc;
	gc.head = NULL;
	gc.roots = NULL;
	gc.paused = 0;

	pid_t pid = fork();

	if (pid == 0)
	{
		// child
		char *child_kept = gc_strdup(&gc, "çocuk kurtulacak");
		char *child_lost = gc_strdup(&gc, "çocuk gidecek");

		gc_track_roots(&gc, &child_kept, (void *)(&child_kept + 1));

		child_lost = NULL;

		gc_collect_unreachable(&gc, __builtin_frame_address(0));

		printf("child kept: %s\n", child_kept);
		printf("child lost is freed\n");

		gc_clear(&gc); // SAKIN UNUTMA
		exit(0);
	}
	else
	{
		wait(NULL);
		char *parent_kept = gc_strdup(&gc, "parent hayatta kalacak");
		char *parent_lost = gc_strdup(&gc, "parent gidecek");

		gc_track_roots(&gc, &parent_kept, (void *)&parent_kept + sizeof(char *));

		parent_lost = NULL;

		gc_collect_unreachable(&gc, __builtin_frame_address(0));

		printf("parent kept: %s\n", parent_kept);
		printf(" parent is freed\n");

		gc_clear(&gc);
	}
}
