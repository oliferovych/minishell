/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:08:26 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/12 16:42:51 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_redirect(t_ast *node)
{
	if (node->type == N_GREAT || node->type == N_LESS || node->type == N_DGREAT)
	{
		if (node->filename)
			free(node->filename);
	}
	else if (node->type == N_DLESS)
	{
		if (node->heredoc)
			free(node->heredoc);
	}
	free(node->tran[0]);
	free(node->tran);
}

void	free_ast(t_ast *node)
{
	int	i;

	i = 0;
	if (!node)
		return ;
	if (node->type == N_COMMAND)
		free_command(node, i);
	else if (node->type == N_GREAT || node->type == N_LESS
		|| node->type == N_DGREAT || node->type == N_DLESS)
		free_redirect(node);
	free_ast(node->left);
	free_ast(node->right);
	free(node);
}

void	free_tokens(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token;
		token = token->next;
		free(tmp->value);
		tmp->value = NULL;
		free(tmp);
	}
}
