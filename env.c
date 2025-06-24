#include "include/main.h"

struct s_global_data g_data;

int main()
{
	char *envis[] = {"HOME=HOME", "PWD=PWD", "CWD=CWD", NULL};
	size_t i;

	i = 0;
	g_data.environs = (char **)malloc(4 * sizeof(char *));
	while (envis[i])
	{
		g_data.environs[i] = ft_strdup(envis[i]);
		i++;
	}
	g_data.environs[i] = NULL;
	g_data.env_size = i;
	print_env();
	unset_env("HOME");
	printf("+__________+\n");
	print_env();
	clear_array(g_data.paths);
	clear_array(g_data.environs);
}