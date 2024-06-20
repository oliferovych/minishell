/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:53:39 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/20 16:44:43 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_strcutoff_front(const char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && str[i] != c)
		i++;
	if (str[i] == c)
		i++;
	while (str[i] != '\0')
	{
		ft_putchar_fd(str[i], 1);
		i++;
	}
}

void	ft_echo(t_ast *ast)
{
	int		i;
	char	*var;
	int		flag;

	flag = 0;
	i = 1;
	if (ft_strnstr(ast->args[i], "-n", 2) != NULL)
	{
		flag++;
		i++;
	}
	while (ast->args[i] != NULL)
	{
		if (ast->args[i][0] == '$')
		{
			var = ft_strtrim(ast->args[i], "$");
			if (variable_exists(ast->ms.env, var))
				print_strcutoff_front(ast->ms.env[variable_exists(ast->ms.env,
						var)], '=');
			else
				ft_printf("%s", ast->args[i]);
		}
		else
			ft_printf("%s", ast->args[i]);
		if (ast->args[i + 1] != NULL)
			write(1, " ", 1);
		i++;
	}
	if (flag == 0)
		ft_printf("\n");
}
