/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:07:24 by tecker            #+#    #+#             */
/*   Updated: 2024/07/09 23:29:54 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *add_quotes(char *str)
{
	char *res;
	int i;
	int j;

	j = 1;
	i = 0;
	res = malloc(ft_strlen(str) + 3);
	if (!res)
		return (NULL);
	res[0] = '\'';
	while (str[i])
		res[j++] = str[i++];
	res[j++] = '\'';
	res[j] = '\0';
	free (str);
	return (res);
}

char	*create_redir_node(t_token **token, t_ast **redir_node)
{
	(*redir_node)->tran = malloc(sizeof(int *));
	if ((*token)->type == T_DLESS)
	{
		(*token) = (*token)->next;
		if (*token && (*token)->type == T_IDENTIFIER)
		{
			(*token)->value = add_quotes((*token)->value);
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

int is_redirection(t_token_type type)
{
	if (type == T_LESS || type == T_GREAT || type == T_DGREAT || type == T_DLESS)
		return (1);
	return (0);
}
