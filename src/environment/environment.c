/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 16:59:42 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/11 16:51:06 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	increment_shlvl(char **environment)
{
	char	*value;
	int		lvl;
	char	*temp;

	value = ft_strcutoff_front(environment[variable_exists(environment,
				"SHLVL")], '=');
	lvl = ft_atoi(value);
	if (lvl > 1000 || lvl <= 0)
		lvl = 1;
	else
		lvl++;
	free(value);
	temp = ft_itoa(lvl);
	value = ft_strjoin("SHLVL=", temp);
	ft_change_existing(value, environment);
	free(value);
	free(temp);
}

void	checkenv(char ***environment)
{
	char	*pwd;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	pwd = ft_strjoin("PWD=", cwd);
	free(cwd);
	if (variable_exists(*environment, "PWD") == -1)
		ft_add_var(pwd, environment);
	if (variable_exists(*environment, "SHLVL") == -1)
		ft_add_var("SHLVL=1", environment);
	else
		increment_shlvl(*environment);
	free(pwd);
}

char	**env_init(char **input_env)
{
	int		i;
	char	**environment;

	i = 0;
	while (input_env[i] != NULL)
		i++;
	environment = malloc((i + 1) * sizeof(char *));
	i = 0;
	if (environment == NULL)
		return (NULL);
	while (input_env[i] != NULL)
	{
		environment[i] = ft_strdup(input_env[i]);
		if (environment[i] == NULL)
		{
			while (i > 0)
				free(environment[--i]);
			return (free(environment), NULL);
		}
		i++;
	}
	environment[i] = NULL;
	checkenv(&environment);
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
