#include "../../includes/minishell.h"

int check_filename(t_ast *ast)
{
	if (ast->filename[0] == '$' && ast->filename[1] && ast->tran[0][0] == 1)
		return(1);
	return (0);
}