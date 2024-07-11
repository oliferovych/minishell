/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 17:28:12 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/10 17:01:39 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*export_line(char *env_line)
{
	char	*line;
	char	*equal_sign;
	size_t	pref_len;
	size_t	suf_len;
	char	*new_env_line;

	equal_sign = strchr(env_line, '=');
	if (equal_sign)
	{
		pref_len = equal_sign - env_line + 1;
		suf_len = strlen(equal_sign + 1);
		new_env_line = (char *)malloc(pref_len + suf_len + 3);
		if (!new_env_line)
			return (NULL);
		ft_strlcpy(new_env_line, env_line, pref_len + 1);
		new_env_line[pref_len] = '\0';
		new_env_line[pref_len] = '"';
		ft_strlcpy(new_env_line + pref_len + 1, equal_sign + 1, suf_len + 1);
		ft_strlcat(new_env_line, "\"", pref_len + suf_len + 3);
		line = ft_strjoin("declare -x ", new_env_line);
		free(new_env_line);
	}
	else
		line = ft_strjoin("declare -x ", env_line);
	return (line);
}

void	print_exp(char **exp)
{
	char	**array_copy;
	int		i;
	int		size;

	i = 0;
	while (exp[i] != NULL)
		i++;
	size = i;
	array_copy = malloc((size + 1) * sizeof(char *));
	if (!array_copy)
	{
		perror("Failed to allocate memory");
		return ;
	}
	i = 0;
	while (exp[i] != NULL)
	{
		array_copy[i] = exp[i];
		i++;
	}
	array_copy[size] = NULL;
	bubble_sort(array_copy, size);
	print_env(array_copy);
	free(array_copy);
}

char	**exp_init(char **environment)
{
	int		i;
	int		count;
	char	**export;

	count = 0;
	while (environment[count] != NULL)
		count++;
	i = 0;
	export = malloc((count + 1) * sizeof(char *));
	if (export == NULL)
		return (NULL);
	while (environment[i] != NULL)
	{
		export[i] = export_line(environment[i]);
		if (export[i] == NULL)
		{
			while (i > 0)
				free(export[--i]);
			free(export);
			return (NULL);
		}
		i++;
	}
	export[i] = NULL;
	return (export);
}
