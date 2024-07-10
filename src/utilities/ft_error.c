/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:09:35 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/10 17:09:37 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_error(t_ast *ast, char *str)
{
	if (str)
		ft_printf("%s\n", str);
	free_all(ast, 1);
	exit(1);
}
