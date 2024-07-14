/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:28:17 by tecker            #+#    #+#             */
/*   Updated: 2024/07/14 17:56:57 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	match_token_help(char *input, int *i)
{
	while (ft_isspace(input[*i]) && input[*i])
		(*i)++;
	if (!input[*i])
		return (2);
	if (((input[*i] == '>' || input[*i] == '<') && (input[*i + 2] == '>'
				|| input[*i + 2] == '<')) || (input[*i] == '>' && input[*i
				+ 1] == '<') || (input[*i] == '<' && input[*i + 1] == '>'))
	{
		write(2, "syntax error near unexpected token ", 35);
		write(2, &input[*i], 1);
		write(2, "\n", 1);
		return (1);
	}
	return (0);
}

int	match_token(char *input, int *i, t_token **lst)
{
	int	res;

	res = match_token_help(input, i);
	if (res || res == 2)
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

t_token	*get_token(char *input)
{
	t_token	*lst;
	int		i;

	i = 0;
	lst = NULL;
	while (input[i])
	{
		if (match_token(input, &i, &lst) == 1)
			return (NULL);
	}
	return (lst);
}
