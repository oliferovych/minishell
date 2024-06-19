/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:53:39 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/17 17:16:04 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_strcutoff_front(const char *str, char c)
{
	int		i;

	i = 0;
	while(str[i] != '\0' && str[i] != c)
		i++;
	if(str[i] == c)
		i++;
	while(str[i] != '\0')
	{
		ft_putchar_fd(str[i], 1);
		i++;
	}
}

void	ft_echo(t_ast *ast)
{
	int		i;
	char	*var;

	if (strcmp(ast->args[1], "-n") == 0)
		i = 2;
	else
		i = 1;
	while (ast->args[i] != NULL)
	{
		if (ast->args[i][0] == '$')
		{
			var = ft_strtrim(ast->args[i], "$");
			if (variable_exists(ast->ms.env, var))
				print_strcutoff_front(ast->ms.env[variable_exists(ast->ms.env, var)], '=');
			else
				ft_printf("%s", ast->args[i]);
		}
		else
			ft_printf("%s", ast->args[i]);
		write (1, " ", 1);
		i++;
	}
	write(1, "\n", 1);
}
