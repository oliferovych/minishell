/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:20:19 by tecker            #+#    #+#             */
/*   Updated: 2024/07/14 22:08:43 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	link_redir_nodes(t_ast **redir_node, t_ast **command_node)
{
	t_ast	*last_redir_node;

	last_redir_node = NULL;
	if (*redir_node)
	{
		last_redir_node = *redir_node;
		while (last_redir_node->left)
			last_redir_node = last_redir_node->left;
		last_redir_node->left = *command_node;
		*command_node = *redir_node;
	}
}

int	count_single_quotes(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			count++;
		i++;
	}
	return (count);
}

char	*transform_variable(char *line, t_ast *ast)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = line;
	line = transform_argiables(line, &i, &ast);
	free(tmp);
	return (line);
}

void	check_for_var(char **res, int flag)
{
	char	*str;

	str = *res;
	if (str[0] == '\"' && str[1] == '$' && str[ft_strlen(str) - 1] == '\"'
		&& str[ft_strlen(str) - 2] == '\'')
		*res = remove_char(ft_strdup(str), '\'');
	if (flag)
		add_double_single_quotes(res);
}

char	*set_str(t_token *token, int *flag)
{
	int	count;
	char *str;

	str = ft_strdup(token->value);
	if (!str)
		return (NULL);
	count = count_single_quotes(str);
	if (count == 4 && str[0] == '\'' && str[1] == '\'' && str[2] == '$'
		&& str[ft_strlen(str) - 1] == '\'' && str[ft_strlen(str) - 2] == '\'')
	{
		str = remove_char(str, '\'');
		*flag = 1;
	}
	else
		*flag = 0;
	return (str);
}
