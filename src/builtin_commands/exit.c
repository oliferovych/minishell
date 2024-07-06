/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:38:22 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/14 20:24:39 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_exit(t_ast *ast)
{
	if (ast->args[1] != NULL)
	{
		if (!ft_atoi(ast->args[1]) || ft_strlen(ast->args[1]) == 0)
		{
			ft_putendl_fd("exit", 2);
			ft_putendl_fd("minishell: exit: numeric argument required", 2);
			free_all(ast, 1);
			exit(255);
		}
		if (ast->args[2])
		{
			ft_putendl_fd("exit", 2);
			ft_putendl_fd("minishell: exit: too many arguments", 2);
			ast->ms.exit_code = 1;
			return ;
		}
		ast->ms.exit_code = ft_atoi(ast->args[1]);
		ast->ms.exit_code = ast->ms.exit_code % 256;
	}
	ft_putendl_fd("exit", 2);
	free_all(ast, 1);
	exit(ast->ms.exit_code);
}
