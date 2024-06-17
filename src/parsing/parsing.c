/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 15:01:17 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/17 15:01:19 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast	*create_ast_node(t_node_type type)
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
	}
	else
		error_indicator(1, "create_node");
	return (node);
}

void	create_node(t_token_type type, t_ast **node)
{
	if (type == T_PIPE)
		*node = create_ast_node(N_PIPE);
	else if (type == T_GREAT)
		*node = create_ast_node(N_GREAT);
	else if (type == T_DGREAT)
		*node = create_ast_node(N_DGREAT);
	else if (type == T_LESS)
		*node = create_ast_node(N_LESS);
	else if (type == T_DLESS)
		*node = create_ast_node(N_DLESS);
	else if (type == T_AND)
		*node = create_ast_node(N_AND);
	else if (type == T_OR)
		*node = create_ast_node(N_OR);
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

void	create_redir_node(t_token **token, t_ast **redir_node)
{
	// if ((*token)->prev && ((*token)->prev->type == T_DLESS))
	// {
	// 	if ((*token)->type == T_IDENTIFIER)
	// 	{
	// 		(*redir_node)->heredoc = ft_strdup((*token)->value);
	// 		if (!(*redir_node)->heredoc)
	// 			error_indicator(1, "Failed to duplicate heredoc");
	// 		(*token) = (*token)->next;
	// 	}
	// 	else
	// 		error_indicator(1, "missing heredoc delimiter after << operator");
	// }
	if ((*token)->type == T_IDENTIFIER)
	{
		(*redir_node)->filename = ft_strdup((*token)->value);
		if (!(*redir_node)->filename)
			error_indicator(1, "Failed to duplicate filename");
	}
	else
		error_indicator(1, "Missing file for redirection");
	*token = (*token)->next;
}

void	create_command_node(t_token **token, t_ast **node)
{
	int		i;
	int		arg_count;
	t_token	*curr_token;

	curr_token = *token;
	arg_count = 0;
	i = 0;
	(*node) = create_ast_node(N_COMMAND);
	(*node)->type = N_COMMAND;
	while (curr_token && curr_token->type == T_IDENTIFIER)
	{
		arg_count++;
		curr_token = curr_token->next;
	}
	(*node)->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!(*node)->args)
		error_indicator(1, "allocating args");
	curr_token = *token;
	while (i < arg_count)
	{
		(*node)->args[i] = ft_strdup(curr_token->value);
		if (!(*node)->args[i])
			error_indicator(1, "duplicating args");
		curr_token = curr_token->next;
		i++;
	}
	(*node)->args[arg_count] = NULL;
	*token = curr_token;
}

void	handle_parentheses(t_token **token, t_ast **node)
{
	(*token) = (*token)->next;
	(*node) = expr(3, token);
	if ((*token) && (*token)->type == T_CPAR)
		(*token) = (*token)->next;
	else
		error_indicator(1, "parenthesis do not close");
}

t_ast	*nud(t_token **token)
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
		create_node((*token)->type, &redir_node);
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
		create_command_node(token, &cmd_node);
		if (prev_redir_node)
			prev_redir_node->left = cmd_node;
		else
			node = cmd_node;
	}
	else if ((*token)->type == T_OPAR)
		handle_parentheses(token, &node);
	return (node);
}

t_ast	*led(t_ast *left, t_token **token)
{
	t_ast	*node;
	int		prec;

	node = NULL;
	prec = get_precedence((*token)->type);
	if (prec > 0)
	{
		create_node((*token)->type, &node);
		node->left = left;
		*token = (*token)->next;
		if (prec == 2)
			create_redir_node(token, &node);
		else
			node->right = expr(prec, token);
	}
	return (node);
}

t_ast	*expr(int prec, t_token **token)
{
	t_ast	*left;

	left = nud(token);
	while (*token && get_precedence((*token)->type) <= prec)
	{
		if ((*token)->type == T_CPAR)
			break ;
		left = led(left, token);
	}
	return (left);
}

t_ast	*parse(t_token **token, char *input, char *prompt, char **env)
{
	t_ast	*node;
	t_token	*tmp;

	tmp = *token;
	node = NULL;
	node = expr(3, token);
	if (node)
	{
		node->ms.token = tmp;
		node->ms.input = input;
		node->ms.prompt = prompt;
		node->ms.env = env;
	}
	if (error_indicator(0, NULL) > 0)
		ft_error(node, "parsing");
	return (node);
}
