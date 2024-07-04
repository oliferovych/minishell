/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 15:03:57 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/17 17:02:13 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	compare_till_in_1(char *str1, char *str2, char c)
{
	int		i;
	char	*temp;
	int		result;

	i = 0;
	temp = ft_strdup(str1);
	while (str1[i] != c && str1[i] != '\0')
		i++;
	if (str1[i] == c)
		temp[i] = '\0';
	result = ft_strcmp(temp, str2);
	free(temp);
	return (result);
}

int	compare_till(char *str1, char *str2, char c)
{
	int		i;
	char	*temp1;
	char	*temp2;
	int		result;

	i = 0;
	temp1 = ft_strdup(str1);
	temp2 = ft_strdup(str2);
	while (str1[i] != c && str1[i] != '\0')
		i++;
	if (str1[i] == c)
		temp1[i] = '\0';
	i = 0;
	while (str2[i] != c && str2[i] != '\0')
		i++;
	if (str2[i] == c)
		temp2[i] = '\0';
	result = ft_strcmp(temp1, temp2);
	free(temp1);
	free(temp2);
	return (result);
}

void	ft_unset(t_ast *ast)
{
	int		i;
	int		j;
	char	*exp_var;

	j = 1;
	while (ast->args[j] != NULL)
	{
		i = 0;
		while (ast->ms.env[i] != NULL)
		{
			if (compare_till_in_1(ast->ms.env[i], ast->args[j], '=') == 0)
				ft_del_var(i, ast->ms.env);
			i++;
		}
		i = 0;
		while (ast->ms.exp[i] != NULL)
		{
			exp_var = ft_strjoin("declare -x ", ast->args[j]);
			if (compare_till_in_1(ast->ms.exp[i], exp_var, '=') == 0)
				ft_del_var(i, ast->ms.exp);
			free(exp_var);
			i++;
		}
		j++;
	}
}
