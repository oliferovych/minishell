/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:31:47 by tecker            #+#    #+#             */
/*   Updated: 2024/07/13 23:57:13 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void double_single_quotes(char **val)
{
	char *new_str;
	int i;
	int j;

	i = 0;
	j = 1;
    if (*val == NULL)
        return ;
    new_str = (char *)malloc(ft_strlen(*val) + 3);
    if (new_str == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_str[0] = '\'';
	while((*val)[i])
	{
		new_str[j] = (*val)[i];
		i++;
		j++;
	}
    new_str[j] = '\'';
    new_str[j + 1] = '\0';
    free(*val);
    *val = new_str;
}

void aa(char **val)
{
	int i;

	if ((*val)[0] == '\"' && (*val)[1] == '\'')
	{
		i = 2;
		while((*val)[i] && (*val)[i] != '\'')
			i++;
		if ((*val)[i] == '\'' && (*val)[i + 1] == '\"')
		{
			double_single_quotes(val);
			return ;
		}
	}
}

void bb(const char *str, int *remove)
{
    int i;

	i = 0;
	if (str[i] == '\'' && str[i + 1] == '\"')
	{
		i += 2;
		while (str[i] && str[i] != '\"')
			i++;
		if (str[i] == '\"' && str[i + 1] == '\'')
		{
			*remove = 0;
			return ;
		}
	}
}



void	check_quotes(char **val, int *quote, int remove)
{
	char	*tmp;
	int		comp_res;

	comp_res = compare_quotes(quote, *val, &remove);
	if ((ft_strstr(*val, "=\"") && quote[1] % 2 == 0)
		|| (ft_strstr(*val, "=\'") && quote[0] % 2 == 0))
	{
		if (ft_strstr(*val, "=\'") && quote[0] % 2 == 0)
			remove = 0;
	}
	else if (((quote[0] % 2 != 0 || quote[1] % 2 != 0) && comp_res))
	{
		ft_printf("unexpected EOF while looking for matching quote\n");
		free (*val);
		*val = NULL;
	}
	bb(*val, &remove);
	aa(val);
	if (remove && comp_res == 0)
		*val = remove_char(*val, '\"');
	else
	{
		tmp = *val;
		*val = ft_strtrim(*val, "\"");
		free(tmp);
	}
}

char	*check_value(char *val, int *quote)
{
	if (!val)
		return (error_indicator(1, "substr"), NULL);
	check_quotes(&val, quote, 1);
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

void	handle_quotes(char **str)
{
	int	i;
	int	quotes;

	quotes = 0;
	i = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == '$')
		{
			while ((*str)[i])
			{
				if (quotes == 2)
					return ;
				if ((*str)[i] == '\"' && ((*str)[i + 1])
					&& (*str)[i + 1] != '$')
				{
					handle_double_quote(str, &i);
					quotes++;
				}
				i++;
			}
		}
		i++;
	}
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
	handle_quotes(&value);
	return (check_value(value, quote));
}
