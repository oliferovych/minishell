/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:59:59 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/17 17:04:24 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_execute_builtin(t_ast *ast, int builtin)
{
	if (builtin == EXIT)
		ft_exit(ast);
	if (builtin == CD)
		ch_dir(ast->args[1]);
	else if (builtin == PWD)
		ft_pwd();
	else if (builtin == ECHO)
		ft_echo(ast);
	else if (builtin == ENV)
		ft_env(ast);
	else if (builtin == UNSET)
		ft_unset(ast);
	else if (builtin == EXPORT)
		ft_exp(ast);
	// else if (strcmp(arguments[0], "exit") == 0)
	// else if (strcmp(arguments[0], "exit") == 0)
	// else if (strcmp(arguments[0], "exit") == 0)
}
