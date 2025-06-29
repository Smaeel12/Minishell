#include <stdlib.h>
#include <stdio.h>

void *return_arr(char **strs, int flag)
{
	if (flag)
		return (strs);
	return (strs[0]);
}

int main()
{

	char **strs = return_arr((char *[]){"hello", "world", NULL}, 1);
	for (size_t i = 0; strs[i]; i++)
		printf("%s\n", strs[i]);
	char *str = return_arr((char *[]){"hello", "world", NULL}, 0);
	printf("%s\n", str);
}