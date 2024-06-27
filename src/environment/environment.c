/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 16:59:42 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/25 14:37:12 by tomecker         ###   ########.fr       */
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
	ft_printf("deleted\n");
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

char	**env_init(char **input_env)
{
	int		i;
	int		count;
	char	**environment;

	count = 0;
	while (input_env[count] != NULL)
		count++;
	i = 0;
	environment = malloc((count + 1) * sizeof(char *));
	if (environment == NULL)
		return (NULL);
	while (input_env[i] != NULL)
	{
		environment[i] = ft_strdup(input_env[i]);
		if (environment[i] == NULL)
		{
			while (i > 0)
				free(environment[--i]);
			free(environment);
			return (NULL);
		}
		i++;
	}
	environment[i] = NULL;
	return (environment);
}

void	print_env(char **environment)
{
	int	i;

	i = 0;
	while (environment[i] != NULL)
	{
		ft_printf("%s\n", environment[i]);
		i++;
	}
}
