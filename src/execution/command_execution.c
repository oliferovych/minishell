/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tecker <tecker@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:13:44 by tecker            #+#    #+#             */
/*   Updated: 2024/07/11 14:14:05 by tecker           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	command_execute(t_ast *ast)
{
	int	builtin;
	int	pid;
	int	status;

	builtin = command_is_builtin(ast->args);
	if (builtin)
		ft_execute_builtin(ast, builtin);
	else
	{
		pid = fork();
		if (pid == -1)
			return (ft_printf("command execution failed\n"), 1);
		if (pid == 0)
			ft_execvp(ast);
		else
		{
			if (waitpid(pid, &status, 0) == -1)
				return (ft_printf("command execution failed\n"), 1);
			ast->ms.exit_code = get_status(status);
		}
	}
	return (0);
}