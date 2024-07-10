/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:58:31 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/10 17:44:45 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_del_var(int i, char **env)
{
	free(env[i]);
	while (env[i] != NULL)
	{
		env[i] = env[i + 1];
		i++;
	}
}

int	variable_exists2(char **env, char *var)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		if (compare_till(env[i], var, '=') == 0)
		{
			free(var);
			return (i);
		}
		i++;
	}
	free(var);
	return (-1);
}

int	variable_exists(char **env, char *var)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		if (compare_till(env[i], var, '=') == 0)
			return (i);
		i++;
	}
	return (-1);
}
