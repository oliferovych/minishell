/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:04:38 by tecker            #+#    #+#             */
/*   Updated: 2024/07/14 23:38:32 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_precedence(t_token_type type)
{
	if (type == T_IDENTIFIER)
		return (0);
	if (type == T_PIPE)
		return (1);
	if (type == T_GREAT || type == T_LESS
		|| type == T_DGREAT || type == T_DLESS)
		return (2);
	if (type == T_AND || type == T_OR)
		return (3);
	if (type == T_OPAR || type == T_CPAR)
		return (4);
	return (-1);
}

t_ast	*nud(t_token **token, t_data *data)
{
	t_ast	*node;
	t_ast	*prev_redir_node;

	node = NULL;
	prev_redir_node = NULL;
	if (handle_r(token, data, &node, &prev_redir_node))
		return (NULL);
	handle_c(token, data, &node, &prev_redir_node);
	return (node);
}

t_ast	*led(t_ast *left, t_token **token, t_data *data)
{
	t_ast	*node;
	int		prec;

	node = NULL;
	prec = get_precedence((*token)->type);
	if (prec > 0)
	{
		create_node((*token)->type, &node, data);
		node->left = left;
		*token = (*token)->next;
		if (prec == 2)
		{
			if (create_redir_node(token, &node) == NULL)
				return (NULL);
		}
		else
		{
			node->right = expr(prec, token, data);
			if (!node->right)
				return (free_ast(node), NULL);
		}
	}
	return (node);
}

t_ast	*expr(int prec, t_token **token, t_data *data)
{
	t_ast	*left;

	left = nud(token, data);
	if (!left)
		return (NULL);
	while (*token && get_precedence((*token)->type) <= prec)
	{
		if ((*token)->type == T_CPAR)
			break ;
		left = led(left, token, data);
		if (!left)
			return (NULL);
	}
	return (left);
}

t_ast	*parse(t_token *token_o, t_data *old_data)
{
	t_ast	*node;
	t_data	*data;
	t_token	*token;

	token = token_o;
	data = NULL;
	node = NULL;
	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->prompt = old_data->prompt;
	data->input = old_data->input;
	data->token = token;
	data->env = old_data->env;
	data->exp = old_data->exp;
	data->exit_code = old_data->exit_code;
	if (!(token))
		return (NULL);
	node = expr(3, &token, data);
	if (!node)
	{
		return (free(data), NULL);
	}
	return (free(data), node);
}
