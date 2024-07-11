/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:13:44 by tecker            #+#    #+#             */
/*   Updated: 2024/07/11 14:31:40 by dolifero         ###   ########.fr       */
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
			return (ft_error(ast, "command execution failed"));
		if (pid == 0)
			ft_execvp(ast);
		else
		{
			if (waitpid(pid, &status, 0) == -1)
				return (ft_error(ast, "command execution failed"));
			ast->ms.exit_code = get_status(status);
		}
	}
	return (0);
}
