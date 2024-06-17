/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:53:39 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/17 17:16:04 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_echo(t_ast *ast)
{
	int		i;
	char	*var;

	if (strcmp(ast->args[1], "-n") == 0)
		i = 2;
	else
		i = 1;
	while (ast->args[i] != NULL)
	{
		if (ast->args[i][0] == '$')
		{
			var = ft_strtrim(ast->args[i], "$");
			if (variable_exists(ast->ms.env, var))
				ft_printf("%s", ast->ms.env[variable_exists(ast->ms.env, var)]);
			else
				ft_printf("%s", ast->args[i]);
		}
		else
			ft_printf("%s", ast->args[i]);
		i++;
	}
	write(1, "\n", 1);
}
