/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:39:23 by tecker            #+#    #+#             */
/*   Updated: 2024/07/11 21:15:53 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	reset_stdin_to_tty(void)
{
	int	reset_stdin;

	reset_stdin = open("/dev/tty", O_RDONLY);
	if (reset_stdin < 0)
		return (ft_error(ft_get_ast(), "redirect failed"));
	dup2(reset_stdin, STDIN_FILENO);
	close(reset_stdin);
	return (0);
}

int	open_heredoc_buffer(int *fd2, int flag)
{
	if (flag)
		*fd2 = open(".heredoc_buffer", O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		*fd2 = open(".heredoc_buffer", O_WRONLY | O_CREAT | O_TRUNC
				| O_APPEND, 0644);
	if (*fd2 < 0)
		return (ft_error(ft_get_ast(), "redirect failed"));
	return (0);
}

int	read_and_write_heredoc(int fd2, t_ast *ast)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (ft_strchr(line, '$'))
			line = transform_variable(line, ast);
		if (!line)
			return (ft_error(ast, "redirect failed"));
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
	return (0);
}

int	set_heredoc_fd(int *fd)
{
	*fd = open(".heredoc_buffer", O_RDONLY);
	if (*fd < 0)
		return (ft_error(ft_get_ast(), "redirect failed"));
	if (dup2(*fd, STDIN_FILENO) < 0)
	{
		close(*fd);
		return (ft_error(ft_get_ast(), "redirect failed"));
	}
	return (0);
}

int	heredoc(int *fd, t_ast *ast, int flag)
{
	int	fd2;

	if (reset_stdin_to_tty())
		return (1);
	if (open_heredoc_buffer(&fd2, flag))
		return (1);
	ft_sigmode_heredoc();
	if (read_and_write_heredoc(fd2, ast))
		return (ft_sigmode_shell(), 1);
	close(fd2);
	if (set_heredoc_fd(fd))
		return (ft_sigmode_shell(), 1);
	ft_sigmode_shell();
	return (0);
}
