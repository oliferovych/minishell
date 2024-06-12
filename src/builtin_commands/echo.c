/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:53:39 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/12 21:15:44 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_echo(char **arguments)
{
	int i;
	int flag;

	i = 1;
	flag = 0;
	if (ft_strnstr(arguments[i], "-n", 2) != NULL)
	{
		flag++;
		i++;
	}
	while(arguments[i + 1])
	{
		ft_printf("%s ", arguments[i]);
		i++;
	}
	ft_printf("%s", arguments[i]);
	if (flag == 0)
		ft_printf("\n");
	
}
