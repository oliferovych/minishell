/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_dir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 16:12:28 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/20 14:40:36 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strcutoff_front(const char *str, char c)
{
	int		i;
	char	*output;
	int		j;

	i = 0;
	while (str[i] != '\0' && str[i] != c)
		i++;
	if (str[i] == c)
		i++;
	output = malloc(sizeof(char) * (ft_strlen(str) - i + 1));
	j = 0;
	while (str[i + j] != '\0')
	{
		output[j] = str[i + j];
		j++;
	}
	output[j] = '\0';
	return (output);
}

void	ch_dir(t_ast *ast)
{
	char	*path;

	if (ast->args[1] == NULL || ft_strcmp(ast->args[1], "~") == 0)
	{
		if (variable_exists(ast->ms.env, "HOME") == -1)
			return ((void)ft_printf("HOME is not set\n"));
		path = ft_strcutoff_front(ast->ms.env[variable_exists(ast->ms.env,
					"HOME")], '=');
		chdir(path);
		free(path);
		return ;
	}
	if (chdir(ast->args[1]) == -1)
	{
		ft_printf("%s: %s\n", strerror(errno), ast->args[1]);
	}
}
