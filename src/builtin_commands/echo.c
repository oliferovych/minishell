/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:53:39 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/10 17:09:59 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_echo(t_ast *ast)
{
	int i;
    int flag;
    i = 1;
    flag = 0;
    if (ast->args[i] && ft_strnstr(ast->args[i], "-n", 2) != NULL)
    {
        flag++;
        i++;
    }
	else if (ast->args[i])
	{
		while(ast->args[i])
		{
			// if (ast->args[i][0] == '$' && ast->args[i][1] == '?')
			// 	ft_printf("%d", ast->ms.exit_code);
			if (!(ast->args[i][0] == '$' && ast->args[i][1]) || ast->tran[i][0] == 0)
				ft_printf("%s", ast->args[i]);
			if (ast->args[i + 1])
				ft_printf(" ");
			i++;
		}
	}
    if (flag == 0)
        ft_printf("\n");
}
