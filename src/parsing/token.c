/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tecker <tecker@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:07:14 by tecker            #+#    #+#             */
/*   Updated: 2024/07/11 14:36:53 by tecker           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*ft_new_token(char *value, t_token_type type)
{
	t_token	*new_token;

	new_token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!new_token)
	{
		error_indicator(1, "calloc");
		return (NULL);
	}
	new_token->value = ft_strdup(value);
	if (!new_token->value)
	{
		error_indicator(1, "dupe value to token");
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	return (new_token);
}

int	add_token(t_token **lst, t_token_type type, char *input, int *i)
{
	t_token	*new;
	char	*value;

	if (type == T_OPAR || type == T_CPAR
		|| type == T_PIPE || type == T_GREAT || type == T_LESS)
	{
		value = ft_substr(input, *i, 1);
		(*i)++;
	}
	else if (type == T_DGREAT || type == T_DLESS
		|| type == T_OR || type == T_AND)
	{
		value = ft_substr(input, *i, 2);
		(*i) += 2;
	}
	else
		value = process_value(input, i);
	if (!value)
		return (1);
	new = ft_new_token(value, type);
	if (!new)
		return (free(value), 1);
	ft_token_list_add_back(lst, new);
	free(value);
	return (0);
}
