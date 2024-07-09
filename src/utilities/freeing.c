/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:08:26 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/09 16:28:33 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_environment(char **environment)
{
	int	i;

	i = 0;
	while (environment[i] != NULL)
	{
		free(environment[i]);
		i++;
	}
	free(environment);
}

void	free_all(t_ast *ast, int flag)
{
	if (ast)
	{
		if (flag)
		{
			free_environment(ast->ms.env);
			free_environment(ast->ms.exp);
		}
		if (ast->ms.input)
			free(ast->ms.input);
		if (ast->ms.prompt)
			free(ast->ms.prompt);
		free_tokens(ast->ms.token);
		free_ast(ast);
	}
}

void	free_command(t_ast *node, int i)
{
	if (node->args)
	{
		while (node->args[i])
		{
			free(node->args[i]);
			if (node->tran[i])
				free(node->tran[i]);
			i++;
		}
		free(node->args);
		free(node->tran);
	}
}

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
