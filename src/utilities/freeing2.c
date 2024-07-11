/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:07:48 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/10 17:09:18 by dolifero         ###   ########.fr       */
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
