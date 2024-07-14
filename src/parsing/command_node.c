/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:04:29 by tecker            #+#    #+#             */
/*   Updated: 2024/07/14 11:47:35 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	while (curr_token && is_redirection(curr_token->type))
	{
		curr_token = curr_token->next;
		if (curr_token && curr_token->type == T_IDENTIFIER)
			curr_token = curr_token->next;
	}
	while (curr_token && curr_token->type == T_IDENTIFIER)
	{
		(*node)->args[i] = transform_arg(node, curr_token, i);
		if (!(*node)->args[i++])
			return ;
		curr_token = curr_token->next;
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
	t_ast	*prev_node;

	prev_node = NULL;
	if (handle_r(token, data, node, &prev_node))
		return (-1);
	while (*token && ((*token)->type == T_IDENTIFIER))
	{
		(arg_count)++;
		*token = (*token)->next;
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
	arg_count = count_args(&curr_token);
	after_token = curr_token;
	arg_count = handle_redir(&after_token, &redir_node, data, arg_count);
	if (arg_count == -1)
		return (NULL);
	(*node) = create_ast_node(N_COMMAND, data);
	if (!allocate_command_memory(node, arg_count))
		return (NULL);
	fill_args(*token, node, arg_count, i);
	*token = after_token;
	link_redir_nodes(&redir_node, node);
	return ("ok");
}
