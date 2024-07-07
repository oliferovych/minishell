/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc_sig.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 00:57:13 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/05 01:50:14 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	heredoc_handler_sigint(int signum)
{
	(void)signum;
	rl_done = 1;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
}
