/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_algo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tecker <tecker@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:07:09 by tecker            #+#    #+#             */
/*   Updated: 2024/07/09 12:42:28 by tecker           ###   ########.fr       */
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
		error_indicator(1, "parenthesis do not close");
}

void	handle_r(t_token **token, t_data *data, t_ast **node, t_ast **prev_node)
{
	t_ast	*redir_node;

	redir_node = NULL;
	while (*token && ((*token)->type == T_LESS || (*token)->type == T_GREAT
			|| (*token)->type == T_DGREAT || (*token)->type == T_DLESS))
	{
		create_node((*token)->type, &redir_node, data);
		create_redir_node(token, &redir_node);
		if (*prev_node)
			(*prev_node)->left = redir_node;
		else
			*node = redir_node;
		(*prev_node) = redir_node;
	}
}

void	handle_c(t_token **token, t_data *data, t_ast **node, t_ast **prev_redi)
{
	t_ast	*cmd_node;

	cmd_node = NULL;
	if ((*token)->type == T_IDENTIFIER)
	{
		create_command_node(token, &cmd_node, data);
		if (*prev_redi)
			(*prev_redi)->left = cmd_node;
		else
			*node = cmd_node;
	}
	else if ((*token)->type == T_OPAR)
		handle_parentheses(token, node, data);
}
