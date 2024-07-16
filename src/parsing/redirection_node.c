/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:07:24 by tecker            #+#    #+#             */
/*   Updated: 2024/07/14 21:27:29 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*add_quotes(char *str)
{
	char	*res;
	int		i;
	int		j;

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
	free(str);
	return (res);
}

char	*handle_heredoc(t_token **token, t_ast **redir_node)
{
	if (*token && (*token)->type == T_IDENTIFIER)
	{
		(*token)->value = add_quotes((*token)->value);
		(*redir_node)->heredoc = transform_arg(redir_node, (*token), 0);
		if (!(*redir_node)->heredoc)
			return (NULL);
	}
	else
	{
		*token = NULL;
		ft_putendl_fd("minishell: missing heredoc delimiter after << operator",
			2);
		return (NULL);
	}
	return ("c");
}

char	*handle_file_redirection(t_token **token, t_ast **redir_node)
{
	if ((*token)->next && (*token)->next->type == T_IDENTIFIER)
	{
		(*token) = (*token)->next;
		(*redir_node)->filename = transform_arg(redir_node, (*token), 0);
		if (!(*redir_node)->filename)
			return (NULL);
	}
	else
	{
		*token = NULL;
		return (ft_putendl_fd("minishell: missing file for redirection", 2),
			NULL);
	}
	return ("c");
}

char	*create_redir_node(t_token **token, t_ast **redir_node)
{
	char	*result;

	result = NULL;
	(*redir_node)->tran = malloc(sizeof(int *));
	if ((*token)->type == T_DLESS)
	{
		(*token) = (*token)->next;
		result = handle_heredoc(token, redir_node);
	}
	else
		result = handle_file_redirection(token, redir_node);
	if (result == NULL)
		return (free((*redir_node)->tran), NULL);
	*token = (*token)->next;
	return (result);
}

int	is_redirection(t_token_type type)
{
	if (type == T_LESS || type == T_GREAT || type == T_DGREAT
		|| type == T_DLESS)
		return (1);
	return (0);
}
