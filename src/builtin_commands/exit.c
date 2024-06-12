/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:38:22 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/12 15:34:29 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_exit(t_ast *ast)
{
	int	exit_code;

	exit_code = 0;
	if (ast->args[1] != NULL)
	{
		if (ast->args[2] != NULL && !ft_atoi(ast->args[1]))
			return ((void)ft_putendl_fd("exit: numeric argument required",
					STDERR_FILENO));
		else if (!atoi(ast->args[1]))
			exit_code = 255;
		else
			exit_code = ft_atoi(ast->args[1]);
		free_all(ast);
		exit(exit_code);
	}
	else
	{
		free_all(ast);
		exit(exit_code);
	}
}
