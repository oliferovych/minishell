/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:53:39 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/21 17:23:08 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_echo(t_ast *ast)
{
	int i;
    int flag;
    i = 1;
    flag = 0;
    if (ft_strnstr(ast->args[i], "-n", 2) != NULL)
    {
        flag++;
        i++;
    }
	if (ast->args[i][0] == '$' && ast->args[i][1] == '?')
		ft_printf("error_code");
	else if (!(ast->args[i][0] == '$' && ast->args[i][1]) || ast->tran[i] == 0)
	{
		while(ast->args[i])
		{
			if (!(ast->args[i][0] == '$' && ast->args[i][1]) || ast->tran[i] == 0)
				ft_printf("%s", ast->args[i]);
			if (ast->args[i + 1] && (!(ast->args[i + 1][0] == '$' && ast->args[i + 1][1]) || ast->tran[i + 1] == 0))
				ft_printf(" ");
			i++;
		}
	}
    if (flag == 0)
        ft_printf("\n");
}
