#include "include/main.h"

int main(int ac, char *av[])
{
	char cmd[] = "'l''s' | 'cat' '";

	t_list *lst = tokenize_cmdline(av[1]);
	ft_lstiter(lst, ft_puts);
}