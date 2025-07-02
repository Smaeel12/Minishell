#include "include/main.h"

struct s_global_data g_data;

int main()
{
	char *expand[3] = {"echo", NULL};
	init_env();
	expand_line(&expand[1], "'$HOME'", (bool[]){1, 1}, 2);
	printf("%s\n", expand[0]);
	printf("%s\n", expand[1]);
	free(expand[1]);
	clean_exit(0);
}