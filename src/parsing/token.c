/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tecker <tecker@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:07:14 by tecker            #+#    #+#             */
/*   Updated: 2024/07/11 13:54:51 by tecker           ###   ########.fr       */
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

int	get_delimiters(int *quotes, char *delimiters, int *remove)
{
	if (quotes[0] % 2 == 0 && quotes[1] % 2 == 0)
		return (0);
	if (quotes[0] % 2 == 0)
	{
		delimiters[0] = '\'';
		delimiters[1] = '\"';
		*remove = 0;
	}
	else
	{
		delimiters[0] = '\"';
		delimiters[1] = '\'';
	}
	return (1);
}

int	compare_quotes(int *quotes, char *val, int *remove)
{
	int		i;
	char	delimiters[2];
	int		open_quotes;

	if (!get_delimiters(quotes, delimiters, remove))
		return (0);
	i = -1;
	open_quotes = 0;
	while (val[++i])
	{
		if (val[i++] == delimiters[0])
		{
			open_quotes++;
			while (val[i] && val[i] != delimiters[0])
			{
				if ((val[i] == '\'' || val[i] == '\"') && open_quotes == 1)
					return (0);
				i++;
			}
			if (!val[i])
				return (1);
			open_quotes++;
		}
	}
	return (1);
}

char	*check_quotes(char *val, int *quote, int remove)
{
	char	*tmp;
	int		comp_res;

	comp_res = compare_quotes(quote, val, &remove);
	if ((ft_strstr(val, "=\"") && quote[1] % 2 == 0)
		|| (ft_strstr(val, "=\'") && quote[0] % 2 == 0))
	{
		if (ft_strstr(val, "=\'") && quote[0] % 2 == 0)
			remove = 0;
	}
	else if (((quote[0] % 2 != 0 || quote[1] % 2 != 0) && comp_res))
	{
		return (ft_printf("unexpected EOF while looking for matching quote\n")
			, NULL);
	}
	if (remove && comp_res == 0)
		val = remove_char(val, '\"');
	else
	{
		tmp = val;
		val = ft_strtrim(val, "\"");
		free(tmp);
	}
	return (val);
}

char	*check_value(char *val, int *quote)
{
	if (!val)
		return (error_indicator(1, "substr"), NULL);
	val = check_quotes(val, quote, 1);
	return (val);
}

void	handle_double_quote(char **str, int *i)
{
	char	*res;
	int		j;
	int		k;

	k = 0;
	j = 0;
	res = malloc(strlen(*str) + 3);
	while ((*str)[k] && k < *i)
		res[j++] = (*str)[k++];
	res[j++] = '\'';
	res[j++] = '\'';
	while ((*str)[k])
		res[j++] = (*str)[k++];
	res[j] = '\0';
	*i = k;
	free(*str);
	*str = res;
}

char	*handle_quotes(char *str)
{
	int	i;
	int	quotes;

	quotes = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			while (str[i])
			{
				if (quotes == 2)
					return (str);
				if (str[i] == '\"' && (str[i + 1]) && str[i + 1] != '$')
				{
					handle_double_quote(&str, &i);
					quotes++;
				}
				i++;
			}
		}
		i++;
	}
	return (str);
}

char	*process_value(char *input, int *i)
{
	char	*value;
	int		j;
	int		quote[2];
	int		count;

	j = *i;
	count = 0;
	quote[0] = 0;
	quote[1] = 0;
	while (input[count + j] && (!ft_isspace(input[count + j])
			|| (quote[0] % 2 != 0 || quote[1] % 2 != 0))
		&& input[j + count] != ')')
	{
		if (input[j + count] == '\'')
			quote[0]++;
		else if (input[j + count] == '\"')
			quote[1]++;
		(count)++;
	}
	*i = j + count;
	value = ft_substr(input, j, count);
	value = handle_quotes(value);
	return (check_value(value, quote));
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

int	match_token_help(char *input, int *i)
{
	while (ft_isspace(input[*i]) && input[*i])
		(*i)++;
	if (!input[*i])
		return (0);
	if (((input[*i] == '>' || input[*i] == '<')
			&& (input[*i + 2] == '>' || input[*i + 2] == '<'))
		|| (input[*i] == '>' && input[*i + 1] == '<')
		|| (input[*i] == '<' && input[*i + 1] == '>'))
		return (ft_printf("syntax error near unexpected token '%c'\n",
				input[*i]), 1);
	return (0);
}

int	match_token(char *input, int *i, t_token **lst)
{
	int	res;

	res = match_token_help(input, i);
	if (res)
		return (res);
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
	else
		return (add_token(lst, T_IDENTIFIER, input, i));
}

t_token	*get_token(char *input, char *prompt)
{
	t_token	*lst;
	int		i;

	i = 0;
	lst = NULL;
	while (input[i])
	{
		if (match_token(input, &i, &lst) == 1)
		{
			free(prompt);
			free(input);
			free_tokens(lst);
			return (NULL);
		}
	}
	return (lst);
}
