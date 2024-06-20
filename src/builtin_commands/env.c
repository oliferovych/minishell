/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 17:59:39 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/14 18:32:50 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_env(t_ast *ast)
{
	if (ast->args[1] == NULL)
		return (print_env(ast->ms.env));
	else
		ft_putendl_fd("Too many arguments.\n", STDERR_FILENO);
	return ;
}
