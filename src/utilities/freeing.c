/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:08:26 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/27 16:24:32 by dolifero         ###   ########.fr       */
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
	if (flag)
	{
		free_environment(ast->ms.env);
		free_environment(ast->ms.exp);
	}
	free(ast->ms.input);
	free(ast->ms.prompt);
	free_tokens(ast->ms.token);
	free_ast(ast);
}
