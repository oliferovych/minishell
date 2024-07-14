/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:53:39 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/14 14:30:03 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	echo_it(t_ast *ast, int i)
{
	while (ast->args[i])
	{
		if (!(ast->args[i][0] == '$' && ast->args[i][1])
			|| ast->tran[i][0] == 0)
			ft_printf("%s", ast->args[i]);
		if (ast->args[i + 1])
			ft_printf(" ");
		i++;
	}
}

void	ft_echo(t_ast *ast)
{
	int	i;
	int	j;
	int	flag;

	i = 1;
	flag = 0;
	while (ast->args[i] && ast->args[i][0] == '-')
	{
		j = 1;
		while (ast->args[i][j] == 'n')
			j++;
		if (ast->args[i][j] == '\0' && ast->args[i][j - 1] == 'n')
			flag++;
		else
			break ;
		i++;
	}
	if (ast->args[i])
		echo_it(ast, i);
	if (flag == 0)
		ft_printf("\n");
}
