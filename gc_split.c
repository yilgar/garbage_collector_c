#include "gc.h"

static int word_count(const char *s, char c)
{
	int i = 0;
	int count = 0;

	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

static char *copy_word(t_gc *gc, const char *s, char c)
{
	int i = 0;
	while (s[i] && s[i] != c)
		i++;

	char *word = gc_malloc(gc, i + 1);
	if (!word)
		return (NULL);

	for (int j = 0; j < i; j++)
		word[j] = s[j];
	word[i] = '\0';

	return word;
}

char **gc_split(t_gc *gc, const char *s, char c)
{
	if (!s)
		return (NULL);

	int count = word_count(s, c);
	char **result = gc_malloc(gc, sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);

	int i = 0, j = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
		{
			result[j++] = copy_word(gc, &s[i], c);
			while (s[i] && s[i] != c)
				i++;
		}
	}
	result[j] = NULL;

	gc_track_roots(gc, result, result + j);

	return result;
}
