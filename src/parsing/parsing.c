#include "../../includes/minishell.h"

t_ast	*create_ast_node(t_node_type type, t_data *data)
{
	t_ast	*node;

	node = (t_ast *)malloc(sizeof(t_ast));
	if (node)
	{
		node->type = type;
		node->args = NULL;
		node->filename = NULL;
		node->heredoc = NULL;
		node->left = NULL;
		node->right = NULL;
		node->ms.token = data->token;
		node->ms.input = data->input;
		node->ms.prompt = data->prompt;
		node->ms.env = data->env;
		node->ms.exp = data->exp;
	}
	else
		error_indicator(1, "create_node");
	return (node);
}

void	create_node(t_token_type type, t_ast **node, t_data *data)
{
	if (type == T_PIPE)
		*node = create_ast_node(N_PIPE, data);
	else if (type == T_GREAT)
		*node = create_ast_node(N_GREAT, data);
	else if (type == T_DGREAT)
		*node = create_ast_node(N_DGREAT, data);
	else if (type == T_LESS)
		*node = create_ast_node(N_LESS, data);
	else if (type == T_DLESS)
		*node = create_ast_node(N_DLESS, data);
	else if (type == T_AND)
		*node = create_ast_node(N_AND, data);
	else if (type == T_OR)
		*node = create_ast_node(N_OR, data);
}

int get_precedence(t_token_type type)
{
	if (type == T_IDENTIFIER)
		return (0);
	if (type == T_PIPE)
		return (1);
	if (type == T_GREAT || type == T_LESS || type == T_DGREAT || type == T_DLESS)
		return(2);
	if	(type == T_AND || type == T_OR)
		return (3);
	if (type == T_OPAR || type == T_CPAR)
        return (4);
	error_indicator(1, "precedence");
	return (-1);
}

void	handle_parentheses(t_token **token, t_ast **node, t_data *data)
{
	(*token) = (*token)->next;
	(*node) = expr(3, token, data);
	if ((*token) && (*token)->type == T_CPAR)
		(*token) = (*token)->next;
	else
		error_indicator(1, "parenthesis do not close");
}

char *ft_str_join_free(char *s1, char *s2)
{
	char *tmp;

	if (!s1 || !s2)
		error_indicator(1, "parsing args");
	tmp = s1;
	s1 = ft_strjoin(tmp, s2);
	if (!s1)
		error_indicator(1, "parsing args");
	free(s2);
	free(tmp);
	return (s1);
}

char *transform_var(t_ast **node, t_token *token, int k)
{
	int i;
	int j;
	char *str;
	char *res;
	char *tmp;

	i = 0;
	res = ft_strdup("");
	if (!res)
        error_indicator(1, "parsing args");
	(*node)->tran[k] = malloc(sizeof(int));
	if (!(*node)->tran[k])
		error_indicator(1, "allocating inner tran");
	(*node)->tran[k][0] = 1;
	str = token->value;
	while(str[i])
	{
		if (str[i] == '\'')
		{
			i++;
			j = i;
			while(str[i] && str[i] != '\'')
				i++;
			tmp = ft_substr(str, j, i - j);
			res = ft_str_join_free(res, tmp);
			if (str[i])
				i++;
			(*node)->tran[k][0] = 0;
		}
		if (ft_strchr(str, '*') != NULL)
		{
			handle_wildcards(str, &res);
			return (res);
		}
		if (str[i] == '$')
		{
			i++;
			j = i;
			while(str[i] && isalnum(str[i]))
				i++;
			if (variable_exists2((*node)->ms.env, ft_substr(str, j, i - j)) != -1)
				tmp = strcutoff_front((*node)->ms.env[variable_exists2((*node)->ms.env, ft_substr(str, j, i - j))], '=');
			else
				tmp = ft_substr(str, j - 1, i - (j - 1));
			res = ft_str_join_free(res, tmp);
		}
		j = i;
		while (str[i] && str[i] != '$' && str[i] != '\'')
			i++;
		tmp = ft_substr(str, j, i - j);
		res = ft_str_join_free(res, tmp);
	}
	return (res);
}

void create_redir_node(t_token **token, t_ast **redir_node)
{
	(*redir_node)->tran = malloc(sizeof(int *));
	if ((*token)->type == T_DLESS)
	{
		(*token) = (*token)->next;
		if (*token && (*token)->type == T_IDENTIFIER)
		{
			// if (ft_strchr(curr_token->value, '*') != NULL)
			// 	(*node)->args[i] = handle_wildcards(curr_token);
			(*redir_node)->heredoc = transform_var(redir_node, (*token), 0);
			if (!(*redir_node)->heredoc)
				error_indicator(1, "duplicating heredoc");
		}
		else
			error_indicator(1, "missing heredoc delimiter after << operator");
	}
	else if ((*token)->next->type == T_IDENTIFIER)
	{
		(*token) = (*token)->next;
		// if (ft_strchr(curr_token->value, '*') != NULL)
		// 	(*node)->args[i] = handle_wildcards(curr_token);
		(*redir_node)->filename = transform_var(redir_node, (*token), 0);
		if (!(*redir_node)->filename)
			error_indicator(1, "duplicating filename");
	}
	else
		error_indicator(1, "Missing file for redirection");
	*token = (*token)->next;
}

void create_command_node(t_token **token, t_ast **node, t_data *data)
{
    int i;
    int arg_count;
    t_token *curr_token;
    t_token *after_token;
	t_ast *redir_node;

	redir_node = NULL;
    curr_token = *token;
    arg_count = 0;
    i = 0;
    (*node) = create_ast_node(N_COMMAND, data);
    while (curr_token && curr_token->type == T_IDENTIFIER)
    {
		arg_count++;
        curr_token = curr_token->next;
    }
    after_token = curr_token;
	if (after_token && (after_token->type == T_LESS || after_token->type == T_GREAT || after_token->type == T_DGREAT || after_token->type == T_DLESS))
	{
		create_node(after_token->type, &redir_node, data);
		create_redir_node(&after_token, &redir_node);
		while (after_token && (after_token->type == T_IDENTIFIER))
		{
			arg_count++;
			after_token = after_token->next;
		}
	}
    (*node)->args = malloc(sizeof(char *) * (arg_count + 1));
    (*node)->tran = malloc(sizeof(int *) * arg_count);
    if (!(*node)->args || !(*node)->tran)
        error_indicator(1, "allocating args");
    curr_token = *token;
    while (curr_token && curr_token->type == T_IDENTIFIER)
    {
		(*node)->args[i] = transform_var(node, curr_token, i);
		if (!(*node)->args[i])
			error_indicator(1, "duplicating args");
        curr_token = curr_token->next;
        i++;
    }
    if (curr_token && (curr_token->type == T_LESS || curr_token->type == T_GREAT || curr_token->type == T_DGREAT))
    {
        if (curr_token->next && curr_token->next->type == T_IDENTIFIER)
        {
            curr_token = curr_token->next->next;
            while (curr_token && curr_token->type == T_IDENTIFIER)
			{
				// if (ft_strchr(curr_token->value, '*') != NULL)
				// 	(*node)->args[i] = handle_wildcards(curr_token);
				(*node)->args[i] = transform_var(node, curr_token, i);
				if (!(*node)->args[i])
					error_indicator(1, "duplicating args");
				curr_token = curr_token->next;
				i++;
			}
        }
    }
    (*node)->args[arg_count] = NULL;
	*token = after_token;
	if (redir_node)
	{
		redir_node->left = *node;
		*node = redir_node;
	}
}

t_ast	*nud(t_token **token, t_data *data)
{
	t_ast	*node;
	t_ast	*cmd_node;
	t_ast	*redir_node;
	t_ast	*prev_redir_node;

	node = NULL;
	cmd_node = NULL;
	redir_node = NULL;
	prev_redir_node = NULL;
	while (*token && ((*token)->type == T_LESS || (*token)->type == T_GREAT
			|| (*token)->type == T_DGREAT || (*token)->type == T_DLESS))
	{
		create_node((*token)->type, &redir_node, data);
		*token = (*token)->next;
		create_redir_node(token, &redir_node);
		if (prev_redir_node)
			prev_redir_node->left = redir_node;
		else
			node = redir_node;
		prev_redir_node = redir_node;
	}
	if ((*token)->type == T_IDENTIFIER)
	{
		create_command_node(token, &cmd_node, data);
		if (prev_redir_node)
			prev_redir_node->left = cmd_node;
		else
			node = cmd_node;
	}
	else if ((*token)->type == T_OPAR)
		handle_parentheses(token, &node, data);
	return (node);
}

t_ast	*led(t_ast *left, t_token **token, t_data *data)
{
	t_ast	*node;
	int		prec;

	node = NULL;
	prec = get_precedence((*token)->type);
	if (prec > 0)
	{
		create_node((*token)->type, &node, data);
		node->left = left;
		*token = (*token)->next;
		if (prec == 2)
			create_redir_node(token, &node);
		else
			node->right = expr(prec, token, data);
	}
	return (node);
}

t_ast	*expr(int prec, t_token **token,  t_data *data)
{
	t_ast	*left;

	left = nud(token, data);
	while (*token && get_precedence((*token)->type) <= prec)
	{
		if ((*token)->type == T_CPAR)
			break ;
		left = led(left, token, data);
	}
	return (left);
}

t_ast	*parse(t_token **token, t_data *old_data)
{
	t_ast	*node;
	t_data  *data;

	data = NULL;
	node = NULL;
	data = malloc(sizeof(t_data));
	if (!data)
		error_indicator(1, NULL);
	else
	{
		data->prompt = old_data->prompt;
		data->input = old_data->input;
		data->token = *token;
		data->env = old_data->env;
		data->exp = old_data->exp;
		node = expr(3, token, data);
	}
	if (error_indicator(0, NULL) > 0)
		ft_error(node, "parsing");
	free(data);
	return (node);
}
