/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_algo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:07:09 by tecker            #+#    #+#             */
/*   Updated: 2024/07/14 22:11:17 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast	*create_ast_node(t_node_type type, t_data *data)
{
	t_ast	*node;

	node = (t_ast *)malloc(sizeof(t_ast));
	if (node)
	{
		node->type = type;
		node->args = NULL;
		node->filename = NULL;
		node->heredoc = NULL;
		node->left = NULL;
		node->right = NULL;
		node->ms.token = data->token;
		node->ms.input = data->input;
		node->ms.prompt = data->prompt;
		node->ms.env = data->env;
		node->ms.exp = data->exp;
		node->ms.exit_code = data->exit_code;
	}
	else
		return (NULL);
	return (node);
}

void	create_node(t_token_type type, t_ast **node, t_data *data)
{
	if (type == T_PIPE)
		*node = create_ast_node(N_PIPE, data);
	else if (type == T_GREAT)
		*node = create_ast_node(N_GREAT, data);
	else if (type == T_DGREAT)
		*node = create_ast_node(N_DGREAT, data);
	else if (type == T_LESS)
		*node = create_ast_node(N_LESS, data);
	else if (type == T_DLESS)
		*node = create_ast_node(N_DLESS, data);
	else if (type == T_AND)
		*node = create_ast_node(N_AND, data);
	else if (type == T_OR)
		*node = create_ast_node(N_OR, data);
}

void	handle_parentheses(t_token **token, t_ast **node, t_data *data)
{
	(*token) = (*token)->next;
	(*node) = expr(3, token, data);
	if ((*token) && (*token)->type == T_CPAR)
		(*token) = (*token)->next;
	else
		ft_putendl_fd("parenthesis do not close", 2);
}

int	handle_r(t_token **token, t_data *data, t_ast **node, t_ast **prev_node)
{
	t_ast	*redir_node;

	redir_node = NULL;
	while (*token && (is_redirection((*token)->type)))
	{
		create_node((*token)->type, &redir_node, data);
		if (create_redir_node(token, &redir_node) == NULL)
			return (free(redir_node), 1);
		if (*prev_node)
			(*prev_node)->left = redir_node;
		else
			*node = redir_node;
		(*prev_node) = redir_node;
	}
	return (0);
}

int	handle_c(t_token **token, t_data *data, t_ast **node, t_ast **prev_redi)
{
	t_ast	*cmd_node;

	cmd_node = NULL;
	if (!(*token))
		return (1);
	if ((*token)->type == T_IDENTIFIER)
	{
		if (!create_command_node(token, &cmd_node, data))
		{
			*node = NULL;
			return (1);
		}
		if (*prev_redi)
			(*prev_redi)->left = cmd_node;
		else
			*node = cmd_node;
	}
	else if ((*token)->type == T_OPAR)
		handle_parentheses(token, node, data);
	return (0);
}
