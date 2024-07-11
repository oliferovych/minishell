/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tecker <tecker@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:39:23 by tecker            #+#    #+#             */
/*   Updated: 2024/07/11 13:41:15 by tecker           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	reset_stdin_to_tty(void)
{
	int	reset_stdin;

	reset_stdin = open("/dev/tty", O_RDONLY);
	if (reset_stdin < 0)
		return (ft_printf("redirect failed\n"), 1);
	dup2(reset_stdin, STDIN_FILENO);
	close(reset_stdin);
	return (0);
}

int	open_heredoc_buffer(int *fd2)
{
	*fd2 = open("heredoc_buffer", O_WRONLY | O_CREAT | O_TRUNC
			| O_APPEND, 0644);
	if (*fd2 < 0)
		return (ft_printf("redirect failed\n"), 1);
	return (0);
}

void	read_and_write_heredoc(int fd2, t_ast *ast)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (ft_strlen(line) == ft_strlen(ast->heredoc)
			&& ft_strncmp(line, ast->heredoc, ft_strlen(ast->heredoc)) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		else
		{
			write(fd2, line, ft_strlen(line));
			write(fd2, "\n", 1);
			if (line)
				free(line);
		}
	}
}

int	set_heredoc_fd(int *fd)
{
	*fd = open("heredoc_buffer", O_RDONLY);
	if (*fd < 0)
		return (ft_printf("redirect failed\n"), 1);
	if (dup2(*fd, STDIN_FILENO) < 0)
	{
		close(*fd);
		return (ft_printf("redirect failed\n"), 1);
	}
	return (0);
}

int	heredoc(int *fd, t_ast *ast)
{
	int	fd2;

	if (reset_stdin_to_tty())
		return (1);
	ft_sigmode_heredoc();
	if (open_heredoc_buffer(&fd2))
		return (1);
	read_and_write_heredoc(fd2, ast);
	close(fd2);
	if (set_heredoc_fd(fd))
		return (1);
	ft_sigmode_shell();
	return (0);
}