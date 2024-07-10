/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_sighandler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 22:12:52 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/10 17:07:11 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	set_sighandler(int signum, void (*handler)(int))
{
	struct sigaction		sa;

	sa.sa_handler = handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (sigaction(signum, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

void	ft_sigmode_shell(void)
{
	set_sighandler(SIGINT, shell_handler_sigint);
	set_sighandler(SIGQUIT, SIG_IGN);
}

void	ft_sigmode_heredoc(void)
{
	set_sighandler(SIGINT, heredoc_handler_sigint);
	set_sighandler(SIGQUIT, SIG_IGN);
}

void	ft_initialize_signals(void)
{
	rl_catch_signals = 0;
	ft_sigmode_shell();
}
