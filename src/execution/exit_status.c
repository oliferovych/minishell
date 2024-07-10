/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 16:34:00 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/10 16:35:43 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_status(int status)
{
	if (WIFSIGNALED(status) && status == SIGTERM)
	{
		ft_putstr_fd("Terminated: ", STDERR_FILENO);
		ft_putnbr_fd(WTERMSIG(status), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (128 + WTERMSIG(status));
	}
	return (WEXITSTATUS(status));
}
