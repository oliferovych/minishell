

#include "../../includes/minishell.h"

void ft_error(t_ast *ast, char *str)
{
	if (str)
		ft_printf("%s\n", str);
	
	free_all(ast, 1);
	exit(1);
}