/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:20:19 by tecker            #+#    #+#             */
/*   Updated: 2024/07/11 20:23:32 by tomecker         ###   ########.fr       */
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
