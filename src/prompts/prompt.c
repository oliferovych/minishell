/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 16:52:33 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/12 21:08:26 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*trim_dir_path(char *dir_path)
{
	int		i;
	int		j;
	char	*trimmed;

	i = ft_strlen(dir_path);
	while (i >= 0 && dir_path[i] != '/')
		i--;
	if (i >= 0)
	{
		trimmed = malloc(ft_strlen(dir_path) - i + 1);
		if (!trimmed)
			return (NULL);
		j = 0;
		while (dir_path[i + j] != '\0')
		{
			trimmed[j] = dir_path[i + j];
			j++;
		}
		trimmed[j] = '\0';
		return (trimmed);
	}
	else
		return (dir_path);
}

char	*get_prompt(void)
{
	char	*prompt;
	char	*dir_path;
	char	*prompt_no_arr;

	dir_path = getcwd(NULL, 0);
	prompt = trim_dir_path(dir_path);
	free(dir_path);
	prompt_no_arr = ft_strjoin("minishell", prompt);
	free(prompt);
	prompt = ft_strjoin(prompt_no_arr, " > ");
	free(prompt_no_arr);
	return (prompt);
}
