/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tecker <tecker@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:07:35 by tecker            #+#    #+#             */
/*   Updated: 2024/07/09 11:07:36 by tecker           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	error_indicator(int i, char *str)
{
	static int	b = 0;

	if (str)
		ft_printf("%s\n", str);
	if (i == 1)
		b++;
	if (i == 3)
		b = 0;
	return (b);
}

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t'
		|| c == '\v');
}

char	*strcutoff_front(const char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && str[i] != c)
		i++;
	if (str[i] == c)
		i++;
	return (ft_substr(str, i, ft_strlen(str) - i));
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

char	*ft_str_join_free(char *s1, char *s2)
{
	char	*tmp;

	if (!s1 || !s2)
	{
		if (s1)
			free(s1);
		else
			free(s2);
		return (NULL);
	}
	tmp = s1;
	s1 = ft_strjoin(tmp, s2);
	if (!s1)
		return (NULL);
	free(s2);
	free(tmp);
	return (s1);
}
