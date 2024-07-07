#include "../../includes/minishell.h"

void	fill_args(t_token *curr_token, t_ast **node, int a_count, int i)
{
	while (curr_token && curr_token->type == T_IDENTIFIER)
	{
		(*node)->args[i] = transform_arg(node, curr_token, i);
		if (!(*node)->args[i++])
			return ;
		curr_token = curr_token->next;
	}
	if (curr_token && (curr_token->type == T_LESS
			|| curr_token->type == T_GREAT || curr_token->type == T_DGREAT))
	{
		if (curr_token->next && curr_token->next->type == T_IDENTIFIER)
		{
			curr_token = curr_token->next->next;
			while (curr_token && curr_token->type == T_IDENTIFIER)
			{
				(*node)->args[i] = transform_arg(node, curr_token, i);
				if (!(*node)->args[i++])
					return ;
				curr_token = curr_token->next;
			}
		}
	}
	(*node)->args[a_count] = NULL;
}

char	*allocate_command_memory(t_ast **node, int arg_count)
{
	(*node)->args = malloc(sizeof(char *) * (arg_count + 1));
	(*node)->tran = malloc(sizeof(int *) * arg_count);
	if (!(*node)->args || !(*node)->tran)
		return (NULL);
	return ("ok");
}

int	count_args(t_token **curr_token)
{
	int	arg_count;

	arg_count = 0;
	while (*curr_token && (*curr_token)->type == T_IDENTIFIER)
	{
		arg_count++;
		*curr_token = (*curr_token)->next;
	}
	return (arg_count);
}

int	handle_redir(t_token **token, t_ast **node, t_data *data, int arg_count)
{
	if (*token && ((*token)->type == T_LESS
			|| (*token)->type == T_GREAT
			|| (*token)->type == T_DGREAT
			|| (*token)->type == T_DLESS))
	{
		create_node((*token)->type, node, data);
		create_redir_node(token, node);
		while (*token && ((*token)->type == T_IDENTIFIER))
		{
			(arg_count)++;
			*token = (*token)->next;
		}
	}
	return (arg_count);
}

char	*create_command_node(t_token **token, t_ast **node, t_data *data)
{
	int		arg_count;
	t_token	*curr_token;
	t_token	*after_token;
	t_ast	*redir_node;
	int		i;

	i = 0;
	redir_node = NULL;
	curr_token = *token;
	arg_count = 0;
	(*node) = create_ast_node(N_COMMAND, data);
	arg_count = count_args(&curr_token);
	after_token = curr_token;
	arg_count = handle_redir(&after_token, &redir_node, data, arg_count);
	if (!allocate_command_memory(node, arg_count))
		return (NULL);
	curr_token = *token;
	fill_args(curr_token, node, arg_count, i);
	*token = after_token;
	if (redir_node)
	{
		redir_node->left = *node;
		*node = redir_node;
	}
	return ("ok");
}
