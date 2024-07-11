/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_checking.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:27:12 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/10 17:36:45 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdint.h>

int	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

int	is_alpha(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int	is_valid_char(char c)
{
	return (is_digit(c) || is_alpha(c) || c == '_');
}

int	check_string(const char *str)
{
	int	i;

	if (is_digit(str[0]))
	{
		ft_putstr_fd("export: ", 2);
		ft_putstr_fd((char *)str, 2);
		ft_putendl_fd(": not a valid identifier", 2);
		return (0);
	}
	i = 0;
	while (str[i] != '\0' && str[i] != '=')
	{
		if (!is_valid_char(str[i]))
		{
			ft_putstr_fd("export: ", 2);
			ft_putstr_fd((char *)str, 2);
			ft_putendl_fd(": not a valid identifier", 2);
			return (0);
		}
		i++;
	}
	return (1);
}
