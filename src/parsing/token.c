#include "../../includes/minishell.h"

t_token	*ft_new_token(char *value, t_token_type type)
{
	t_token	*new_token;

	new_token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!new_token)
		error_indicator(1, "calloc");
	new_token->value = ft_strdup(value);
	if (!new_token->value)
		error_indicator(1, "dupe value to token");
	new_token->type = type;
	return (new_token);
}

void	ft_token_list_add_back(t_token **lst, t_token *new_token)
{
	t_token	*curr_node;

	if (!*lst)
	{
		*lst = new_token;
		return ;
	}
	curr_node = *lst;
	while (curr_node && curr_node->next)
		curr_node = curr_node->next;
	curr_node->next = new_token;
	new_token->prev = curr_node;
}

char *process_value(char *input, int *i, int *count)
{
    char *value;
    int	j;
	int single;
	int doublee;
	int quote;
	
	quote = 0;
	doublee = 0;
	single = 0;
	j = *i;
	while (input[*count + j] && (!ft_isspace(input[*count + j]) || quote == 1) && input[j + *count] != ')')
	{
		if (input[j + *count] == '\'')
		{
			single++;
			quote++;
		}
		else if (input[j + *count] == '\"')
		{
			doublee++;
			quote++;
		}
		(*count)++;
	}
	*i = j + *count;
    value = ft_substr(input, j, *count);
    if (!value)
        error_indicator(1, "substr");
	if (doublee % 2 != 0 || single % 2 != 0)
        error_indicator(1, "unexpected EOF while looking for matching quote");
    return (value);
}

int	add_token(t_token **lst, t_token_type type, char *input, int i)
{
	t_token	*new;
	char	*value;
	int		count;

	count = 0;
	if (type == T_OPAR || type == T_CPAR)
	{
		count++;
		value = ft_substr(input, i, count);
	}
	else
	{
		value = process_value(input, &i, &count);
		value = remove_char(value, '\"');
	}

	new = ft_new_token(value, type);
	ft_token_list_add_back(lst, new);
	free(value);
	return (i);
}

t_token	*get_token(char *input, char *prompt)
{
	t_token	*lst;
	int		i;

	i = 0;
	lst = NULL;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (input[i] == '|' && input[i + 1] != '|')
			i = add_token(&lst, T_PIPE, input, i);
		else if (input[i] == '>' && input[i + 1] == '>')
			i = add_token(&lst, T_DGREAT, input, i);
		else if (input[i] == '>')
			i = add_token(&lst, T_GREAT, input, i);
		else if (input[i] == '<' && input[i + 1] == '<')
			i = add_token(&lst, T_DLESS, input, i);
		else if (input[i] == '<')
			i = add_token(&lst, T_LESS, input, i);
		else if (input[i] == '|' && input[i + 1] == '|')
			i = add_token(&lst, T_OR, input, i);
		else if (input[i] == '&' && input[i + 1] == '&')
			i = add_token(&lst, T_AND, input, i);
		else if (input[i] == '(')
			i = add_token(&lst, T_OPAR, input, i);
		else if (input[i] == ')')
			i = add_token(&lst, T_CPAR, input, i);
		else if (input[i])
			i = add_token(&lst, T_IDENTIFIER, input, i);
	}
	if (error_indicator(0, NULL) > 0)
	{
		free(prompt);
		free(input);
		free_tokens(lst);
		exit(1);
	}
	return (lst);
}
