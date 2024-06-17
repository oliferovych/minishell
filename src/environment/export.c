/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 17:28:12 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/17 15:13:08 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		export[i] = ft_strjoin("declare -x ", environment[i]);
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
