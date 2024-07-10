/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:09:47 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/10 17:09:49 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_filename(t_ast *ast)
{
	if (ast->filename[0] == '$' && ast->filename[1] && ast->tran[0][0] == 1)
		return (1);
	return (0);
}

char	*remove_char(const char *str, char char_to_remove)
{
	int		new_len;
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	new_len = 0;
	if (!str)
		return (NULL);
	while (str[i] != '\0')
		if (str[i++] != char_to_remove)
			new_len++;
	new_str = (char *)malloc(new_len + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		if (str[i] != char_to_remove)
			new_str[j++] = str[i];
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

void	free_double_array_char(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}
