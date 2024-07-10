/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:15:46 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/10 17:06:32 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_add_var(char *var, char ***env)
{
	int		i;
	char	**new_env;
	int		j;

	i = 0;
	while ((*env)[i] != NULL)
		i++;
	new_env = malloc((i + 2) * sizeof(char *));
	if (!new_env)
	{
		perror("allocation failed\n");
		return ;
	}
	j = 0;
	while (j < i)
	{
		new_env[j] = (*env)[j];
		j++;
	}
	new_env[i] = ft_strdup(var);
	new_env[i + 1] = NULL;
	free(*env);
	*env = new_env;
}

void	ft_change_existing(char *var, char **env)
{
	int	i;

	i = variable_exists(env, var);
	free(env[i]);
	env[i] = ft_strdup(var);
}

static void	add_export(t_ast *ast, char *exp)
{
	if (variable_exists(ast->ms.exp, exp) == -1)
		ft_add_var(exp, &ast->ms.exp);
	else
		ft_change_existing(exp, ast->ms.exp);
	free(exp);
}

void	ft_exp(t_ast *ast)
{
	int		i;
	char	*exp;

	if (ast->args[1] == NULL)
		return (print_exp(ast->ms.exp));
	else
	{
		i = 1;
		while (ast->args[i] != NULL)
		{
			if (ft_strchr(ast->args[i], '='))
			{
				if (variable_exists(ast->ms.env, ast->args[i]) == -1)
					ft_add_var(ast->args[i], &ast->ms.env);
				else
					ft_change_existing(ast->args[i], ast->ms.env);
			}
			exp = export_line(ast->args[i]);
			add_export(ast, exp);
			i++;
		}
	}
}
