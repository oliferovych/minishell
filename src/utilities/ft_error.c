/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:09:35 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/11 14:29:49 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_error(t_ast *ast, char *str)
{
	ast->ms.exit_code = 1;
	ft_putstr_fd("minishell: ", 2);
	ft_putendl_fd(str, 2);
	return (1);
}
