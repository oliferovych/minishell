/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 16:59:42 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/09 16:40:56 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
