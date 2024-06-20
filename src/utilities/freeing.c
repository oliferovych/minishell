/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:08:26 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/14 20:24:11 by tomecker         ###   ########.fr       */
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

void	free_export(char **export)
{
	int	i;

	i = 0;
	while (export[i] != NULL)
	{
		free(export[i]);
		i++;
	}
	free(export);
}

void	free_all(t_ast *ast, int flag)
{
	// free_export(ast->ms.exp);
	if (flag)
		free_environment(ast->ms.env);
	free(ast->ms.input);
	free(ast->ms.prompt);
	free_tokens(ast->ms.token);
	free_ast(ast);
}
