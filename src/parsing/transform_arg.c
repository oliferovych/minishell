/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_arg.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:07:19 by tecker            #+#    #+#             */
/*   Updated: 2024/07/14 22:09:27 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*transform_arg_single_quote(char *str, int *i, int k, t_ast **node)
{
	int		j;
	char	*tmp;
	int		count;

	count = count_single_quotes(str);
	if (count % 2 == 0)
		(*i)++;
	j = *i;
	if (count % 2 != 0 || count == 4)
		(*i)++;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	if (count == 4)
		(*i)++;
	tmp = ft_substr(str, j, *i - j);
	if (str[*i])
		(*i)++;
	(*node)->tran[k][0] = 0;
	return (tmp);
}

char	*transform_argiables(char *str, int *i, t_ast **node)
{
	int		j;
	char	*tmp;

	(*i)++;
	j = *i;
	while (str[*i] && (isalnum(str[*i])))
		(*i)++;
	if (str[*i] == '?')
	{
		tmp = ft_itoa((*node)->ms.exit_code);
		(*i)++;
		return (tmp);
	}
	if (variable_exists2((*node)->ms.env, ft_substr(str, j, *i - j)) != -1)
		tmp = strcutoff_front((*node)->ms.env[variable_exists2((*node)->ms.env,
					ft_substr(str, j, *i - j))], '=');
	else
		tmp = ft_substr(str, j - 1, *i - (j - 1));
	return (tmp);
}

char	*transform_other(char *str, int *i)
{
	int		j;
	char	*tmp;

	j = *i;
	while (str[*i] && str[*i] != '$' && str[*i] != '\'')
		(*i)++;
	tmp = ft_substr(str, j, *i - j);
	return (tmp);
}

char	*transform_arg_sub(t_ast **node, char *str, int k, char *res)
{
	char	*tmp;
	int		i;

	i = 0;
	while (str[i])
	{
		if (!res)
			return (NULL);
		if (str[i] == '\'')
			res = ft_str_join_free(res, transform_arg_single_quote
					(str, &i, k, node));
		if (str[i] == '*')
		{
			tmp = handle_wildcards(str);
			if (tmp)
				return (ft_str_join_free(res, tmp));
		}
		if (str[i] == '$')
			res = ft_str_join_free(res, transform_argiables(str, &i, node));
		tmp = transform_other(str, &i);
		res = ft_str_join_free(res, tmp);
	}
	return (res);
}

char	*transform_arg(t_ast **node, t_token *token, int k)
{
	char	*res;
	char	*str;
	int		flag;

	
	str = set_str(token, &flag);
	res = ft_strdup("");
	(*node)->tran[k] = malloc(sizeof(int));
	if (!(*node)->tran[k] || !res)
		return (NULL);
	(*node)->tran[k][0] = 1;
	res = transform_arg_sub(node, str, k, res);
	check_for_var(&res, flag);
	free(str);
	return (res);
}
