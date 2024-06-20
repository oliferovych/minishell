/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 15:01:17 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/19 14:45:48 by tomecker         ###   ########.fr       */
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
	}
	else
		error_indicator(1, "create_node");
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

int	get_precedence(t_token_type type)
{
	if (type == T_IDENTIFIER)
		return (0);
	if (type == T_PIPE)
		return (1);
	if (type == T_GREAT || type == T_LESS || type == T_DGREAT
		|| type == T_DLESS)
		return (2);
	if (type == T_AND || type == T_OR)
		return (3);
	if (type == T_OPAR || type == T_CPAR)
		return (4);
	error_indicator(1, "precedence");
	return (-1);
}

void handle_parentheses(t_token **token, t_ast **node)
{
	(*token) = (*token)->next;
	(*node) = expr(3, token);
	if ((*token) && (*token)->type == T_CPAR)
		(*token) = (*token)->next;
	else
		error_indicator(1, "parenthesis do not close");
}

void	create_redir_node(t_token **token, t_ast **redir_node)
{
	if ((*token)->type == T_DLESS)
	{
		(*token) = (*token)->next;
		if (*token && (*token)->type == T_IDENTIFIER)
		{
			(*redir_node)->heredoc = ft_strdup((*token)->value);
			if (!(*redir_node)->heredoc)
				error_indicator(1, "Failed to duplicate heredoc");
		}
		else
			error_indicator(1, "missing heredoc delimiter after << operator");
	}
	else if ((*token)->next->type == T_IDENTIFIER)
	{
		(*token) = (*token)->next;
		(*redir_node)->filename = ft_strdup((*token)->value);
		if (!(*redir_node)->filename)
			error_indicator(1, "Failed to duplicate filename");
	}
	else
		error_indicator(1, "Missing file for redirection");
	*token = (*token)->next;
}

void	create_command_node(t_token **token, t_ast **node, t_data *data)
{
	int		i;
	int		arg_count;
	t_token	*curr_token;

	curr_token = *token;
	arg_count = 0;
	i = 0;
	(*node) = create_ast_node(N_COMMAND, data);
	(*node)->type = N_COMMAND;
	while (curr_token && curr_token->type == T_IDENTIFIER)
	{
		create_node(after_token->type, &redir_node);
		create_redir_node(&after_token, &redir_node);
		while (after_token && (after_token->type == T_IDENTIFIER))
		{
			arg_count++;
			after_token = after_token->next;
		}
	}
    (*node)->args = malloc(sizeof(char *) * (arg_count + 1));
    if (!(*node)->args)
        error_indicator(1, "allocating args");
    curr_token = *token;
    while (curr_token && curr_token->type == T_IDENTIFIER)
    {
        (*node)->args[i] = ft_strdup(curr_token->value);
        if (!(*node)->args[i])
            error_indicator(1, "duplicating args");
        curr_token = curr_token->next;
        i++;
    }
    if (curr_token && (curr_token->type == T_LESS || curr_token->type == T_GREAT || curr_token->type == T_DGREAT))
    {
        if (curr_token->next && curr_token->next->type == T_IDENTIFIER)
        {
            curr_token = curr_token->next->next;
            while (curr_token && curr_token->type == T_IDENTIFIER)
            {
                (*node)->args[i] = ft_strdup(curr_token->value);
                if (!(*node)->args[i])
                    error_indicator(1, "duplicating args");
                curr_token = curr_token->next;
                i++;
            }
        }
    }
    (*node)->args[arg_count] = NULL;
	*token = after_token;
	if (redir_node)
	{
		redir_node->left = *node;
		*node = redir_node;
	}
	(*node)->args[arg_count] = NULL;
	*token = curr_token;
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

t_ast	*nud(t_token **token, t_data *data)
{
	t_ast	*node;
	t_ast	*cmd_node;
	t_ast	*redir_node;
	t_ast	*prev_redir_node;

	node = NULL;
	cmd_node = NULL;
	redir_node = NULL;
	prev_redir_node = NULL;
	while (*token && ((*token)->type == T_LESS || (*token)->type == T_GREAT
			|| (*token)->type == T_DGREAT || (*token)->type == T_DLESS))
	{
		create_node((*token)->type, &redir_node, data);
		*token = (*token)->next;
		create_redir_node(token, &redir_node);
		if (prev_redir_node)
			prev_redir_node->left = redir_node;
		else
			node = redir_node;
		prev_redir_node = redir_node;
	}
	if ((*token)->type == T_IDENTIFIER)
	{
		create_command_node(token, &cmd_node, data);
		if (prev_redir_node)
			prev_redir_node->left = cmd_node;
		else
			node = cmd_node;
	}
	else if ((*token)->type == T_OPAR)
		handle_parentheses(token, &node, data);
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
			create_redir_node(token, &node);
		else
			node->right = expr(prec, token, data);
	}
	return (node);
}

t_ast	*expr(int prec, t_token **token,  t_data *data)
{
	t_ast	*left;

	left = nud(token, data);
	while (*token && get_precedence((*token)->type) <= prec)
	{
		if ((*token)->type == T_CPAR)
			break ;
		left = led(left, token, data);
	}
	return (left);
}

t_ast	*parse(t_token **token, char *input, char *prompt, char **env)
{
	t_ast	*node;
	t_data *data;
	
	data = NULL;
	node = NULL;
	data = malloc(sizeof(t_data));
	if (!data)
		error_indicator(1, NULL);
	else
	{
		data->prompt = prompt;
		data->input = input;
		data->token = *token;
		data->env = env;
		node = expr(3, token, data);
	}
	if (error_indicator(0, NULL) > 0)
		ft_error(node, "parsing");
	free(data);
	return (node);
}
