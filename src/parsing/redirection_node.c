#include "../../includes/minishell.h"

char	*create_redir_node(t_token **token, t_ast **redir_node)
{
	(*redir_node)->tran = malloc(sizeof(int *));
	if ((*token)->type == T_DLESS)
	{
		(*token) = (*token)->next;
		if (*token && (*token)->type == T_IDENTIFIER)
		{
			(*redir_node)->heredoc = transform_arg(redir_node, (*token), 0);
			if (!(*redir_node)->heredoc)
				return (NULL);
		}
		else
			error_indicator(1, "missing heredoc delimiter after << operator");
	}
	else if ((*token)->next->type == T_IDENTIFIER)
	{
		(*token) = (*token)->next;
		(*redir_node)->filename = transform_arg(redir_node, (*token), 0);
		if (!(*redir_node)->filename)
			return (NULL);
	}
	else
		error_indicator(1, "Missing file for redirection");
	*token = (*token)->next;
	return ("c");
}
