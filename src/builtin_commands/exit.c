/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tecker <tecker@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:38:22 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/12 12:42:32 by tecker           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_exit(t_ast *ast)
{
	int	exitcode;

	ast->ms.exit_code = 0;
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
		}
		ast->ms.exit_code = ft_atoi(ast->args[1]);
		ast->ms.exit_code = ast->ms.exit_code % 256;
	}
	exitcode = ast->ms.exit_code;
	ft_putendl_fd("exit", 2);
	free_all(ast, 1);
	exit(exitcode);
}
