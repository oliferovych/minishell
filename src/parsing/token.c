/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tecker <tecker@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:07:14 by tecker            #+#    #+#             */
/*   Updated: 2024/07/09 11:07:15 by tecker           ###   ########.fr       */
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

char	*process_value(char *input, int *i, int *count)
{
	char	*value;
	int		j;
	int		quote[2];

	j = *i;
	quote[0] = 0;
	quote[1] = 0;
	while (input[*count + j] && (!ft_isspace(input[*count + j])
			|| (quote[0] % 2 != 0 || quote[1] % 2 != 0))
		&& input[j + *count] != ')')
	{
		if (input[j + *count] == '\'')
			quote[0]++;
		else if (input[j + *count] == '\"')
			quote[1]++;
		(*count)++;
	}
	*i = j + *count;
	value = ft_substr(input, j, *count);
	if (!value)
		return (error_indicator(1, "substr"), NULL);
	if (quote[0] % 2 != 0 || quote[1] % 2 != 0)
		return (ft_printf("unexpected EOF while looking for matching quote")
			, NULL);
	return (value);
}

int	add_token(t_token **lst, t_token_type type, char *input, int *i)
{
	t_token	*new;
	char	*value;
	int		count;

	count = 0;
	if (type == T_OPAR || type == T_CPAR)
	{
		count++;
		value = ft_substr(input, *i, count);
	}
	else
	{
		value = process_value(input, i, &count);
		value = remove_char(value, '\"');
	}
	if (!value)
		return (1);
	new = ft_new_token(value, type);
	if (!new)
		return (free(value), 1);
	ft_token_list_add_back(lst, new);
	free(value);
	return (0);
}

int	match_token(char *input, int *i, t_token **lst)
{
	while (ft_isspace(input[*i]))
		(*i)++;
	if (input[*i] == '|' && input[*i + 1] != '|')
		return (add_token(lst, T_PIPE, input, i));
	else if (input[*i] == '>' && input[*i + 1] == '>')
		return (add_token(lst, T_DGREAT, input, i));
	else if (input[*i] == '>')
		return (add_token(lst, T_GREAT, input, i));
	else if (input[*i] == '<' && input[*i + 1] == '<')
		return (add_token(lst, T_DLESS, input, i));
	else if (input[*i] == '<')
		return (add_token(lst, T_LESS, input, i));
	else if (input[*i] == '|' && input[*i + 1] == '|')
		return (add_token(lst, T_OR, input, i));
	else if (input[*i] == '&' && input[*i + 1] == '&')
		return (add_token(lst, T_AND, input, i));
	else if (input[*i] == '(')
		return (add_token(lst, T_OPAR, input, i));
	else if (input[*i] == ')')
		return (add_token(lst, T_CPAR, input, i));
	else if (input[*i])
		return (add_token(lst, T_IDENTIFIER, input, i));
	return (1);
}

t_token	*get_token(char *input, char *prompt)
{
	t_token	*lst;
	int		i;

	i = 0;
	lst = NULL;
	while (input[i])
	{
		if (match_token(input, &i, &lst))
		{
			free(prompt);
			free(input);
			free_tokens(lst);
			exit(1);
		}
	}
	return (lst);
}
