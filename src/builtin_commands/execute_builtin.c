/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tecker <tecker@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:59:59 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/12 12:30:47 by tecker           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_execute_builtin(t_ast *ast, int builtin)
{
	if (builtin == EXIT)
		ft_exit(ast);
	if (builtin == CD)
		ch_dir(ast);
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
}
