#include "gc.h"

/*  test case 1, düz gc_strdup valgrind temiz */
// int main(int ac, char **av, char **ev)
// {
// 	t_gc	gc = {0};
// 	char	*str1;
// 	str1 = NULL;

// 	(void)ac;
// 	(void)av;
// 	(void)ev;

// 	str1 = gc_strdup(&gc, "testdeneme");

// 	printf("%s\n", str1);

// 	gc_clear(&gc);
// 	return (0);
// }



/* test case 2, iki tane allocate edip aradan bir tane silmek, valgrind temiz*/
// int	main(int ac, char **av, char **ev)
// {
// 	t_gc	gc = {0};
// 	char	*msg;
// 	char	*line;

// 	(void)ac;
// 	(void)av;
// 	(void)ev;

// 	msg = gc_strdup(&gc, "hello");
// 	line = gc_strdup(&gc, "heredoc content");
// 	printf("msg: %s\n", msg);
// 	printf("line: %s\n", line);
// 	gc_free_one(&gc, line); // sadece line free’lenecek
// 	gc_clear(&gc); // geri kalan her şey temizlenecek
// }



/*şaibeli fix lazım olabilir tekrar bakacağım*/
#include "gc.h"

// int	main(void)
// {
// 	t_gc	gc;
// 	char	*kept;
// 	char	*lost;
// 	char	*test;

// 	// GC başlat
// 	gc.head = NULL;
// 	gc.roots = NULL;

// 	// GC yönetiminde str'ler
// 	kept = gc_strdup(&gc, "kept value");
// 	lost = gc_strdup(&gc, "lost value");
// 	test = gc_strdup(&gc, "temp string");

// 	printf("--- Toplama Öncesi ---\n");
// 	printf("Kept: %p -> %s\n", (void *)kept, kept);
// 	printf("Lost: %p -> %s\n", (void *)lost, lost);
// 	printf("Test: %p -> %s\n", (void *)test, test);
// 	printf("----------------------\n\n");

// 	gc_track_roots(&gc, &kept, (void *)&kept + sizeof(char *)); // Daha güvenli bir kullanım

// 	// 'lost' ve 'test' pointer'larını stack'ten temizleyerek
// 	// ulaşılamaz hale getiriorz.
// 	lost = NULL;
// 	test = NULL;
// 	// __builtin_frame_address(0) main'in stack frame'inin başlangıcı
// 	// Bu adresin altındaki (daha yüksek adres) stack alanları taranır.
// 	gc_collect_unreachable(&gc, __builtin_frame_address(0));

// 	if (kept)
// 		printf("%s\n", kept);
// 	else
// 		printf("freed\n");

// 	if (lost)
// 		printf("%s\n", lost);
// 	else
// 		printf("freed\n");

// 	if (test) // Bu her zaman false olacak
// 		printf("%s \n", test);
// 	else
// 		printf("freed\n");
// 	printf("----------------------\n");

// 	gc_clear(&gc);
// 	return (0);
// }

// int	main(void)
// {
// 	t_gc	gc = {0};
// 	gc.head = NULL;
// 	gc.roots = NULL;
// 	gc.paused = 0;

// 	char	**result;
// 	char	*str;
// 	void	*stack_bottom;

// 	str = NULL;
// 	result = NULL;
	
// 	stack_bottom = __builtin_frame_address(0);
// 	str = "merhaba bu bir testtir";

// 	// Burada local değişken adresi kullan, daha güvenli

// 	result = gc_split(&gc, str, ' ');
// 	gc_track_roots(&gc, &result, (void *)(&result + 1));

// 	for (int i = 0; result[i]; i++)
// 		printf("token[%d] = %s\n", i, result[i]);

// 	// Asıl kritik kısım burada, fonksiyon içine stack referansı yolla
// 	gc_collect_unreachable(&gc, stack_bottom);

// 	gc_clear(&gc);
// 	return (0);
// }


// void gc_init(t_gc *gc)
// {
// 	gc->head = NULL;
// 	gc->roots = NULL;
// 	gc->paused = 0;
// }

// int main(void)
// {
// 	t_gc *gc = malloc(sizeof(t_gc));
// 	void *stack_top = __builtin_frame_address(0);

// 	if (!gc)
// 		return (1); // malloc başarısızsa çık

// 	// initialize garbage collector (boş root/head/null yap, vs.)
// 	gc_init(gc); // bu fonksiyonu senin tanımlamış olman gerek

// 	char *line = gc_strdup(gc, "merhaba bu bir testtir");
// 	char **tokens = gc_split(gc, line, ' ');

// 	for (int i = 0; tokens[i]; i++)
// 		printf("token[%d] = %s\n", i, tokens[i]);

// 	gc_collect_unreachable(gc, stack_top);

// 	for (int i = 0; tokens[i]; i++)
// 		printf("token[%d] after GC: %s\n", i, tokens[i]);

// 	gc_clear(gc); // tüm memory free eder
// 	free(gc);     // en son kendisini de serbest bırak
// }



int main()
{
	t_gc gc = {0};

	gc.head = NULL;
	gc.roots = NULL;
	gc.paused = 0;

	//without define __builtin_frame_address to any void *,
	//it'll cause to not initialised values in valgrind, ++asan clear..
	void *stack_top = __builtin_frame_address(0);

	char *kept = gc_strdup(&gc, "kaşık");
	char *str = gc_strdup(&gc, "gidecek");
	char *str1 = gc_strdup(&gc, "elma");

	printf(" %s .. track öncesi1\n", kept);
	printf(" %s .. track öncesi1\n", str);
	printf(" %s .. track öncesi1\n", str1);
	gc_track_roots(&gc, &kept, (void *)&kept + sizeof(char *));

	
	printf("%s .. track sonrası2\n", kept);
	printf("%s .. track sonrası2\n", str);
	printf("%s .. track sonrası2\n", str1);

	
	gc_collect_unreachable(&gc, stack_top);
	str = NULL;
	str1 = NULL;

	
	if (kept)
		printf("%s .. unreachable sonrası3\n", kept);
	else
		printf("freed\n");
		
	if (str)
		printf("%s .. unreachable sonrası3\n", str);
	else
		printf("freed\n");
	if (str1)
		printf("%s .. unreachable sonrası3\n", str1);
	else
		printf("freed\n");

	gc_clear(&gc);
}

