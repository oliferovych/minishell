/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:09:27 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/10 16:53:13 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	command_execute(t_ast *ast)
{
	int	builtin;
	int	pid;
	int	status;

	builtin = command_is_builtin(ast->args);
	if (builtin)
		ft_execute_builtin(ast, builtin);
	else
	{
		pid = fork();
		if (pid == -1)
			ft_error(ast, "fork");
		if (pid == 0)
			ft_execvp(ast);
		else
		{
			if (waitpid(pid, &status, 0) == -1)
				ft_error(ast, "waitpid");
			ast->ms.exit_code = get_status(status);
		}
	}
}

int redirect(t_ast *ast)
{
	int fd;
	int fd2;
	int res;
	char *line;

	if (ast->filename && check_filename(ast))
	{
		ft_printf("%s: ambiguous redirect\n", ast->filename);
		return (1);
	}
	if (ast->type == N_LESS)
	{
		fd = open(ast->filename, O_RDONLY);
		if (fd < 0)
			ft_error(ast, "redirection");
		res = dup2(fd, STDIN_FILENO);
		if (res < 0)
		{
			close(fd);
			ft_error(ast, "redirection");
		}
	}
	else if (ast->type == N_DLESS)
	{
		fd2 = open("heredoc_buffer", O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0644);
		if (fd2 < 0)
			ft_error(ast, "redirection");
		while (1)
		{
			line = readline("> ");
			if (line && ft_strncmp(line, ast->heredoc, ft_strlen(line)) != 0)
			{
				write(fd2, line, ft_strlen(line));
				write(fd2, "\n", 1);
			}
			else
				break;
			if (line)
				free(line);
		}

		close(fd2);
		fd = open("heredoc_buffer", O_RDONLY);
		if (fd < 0)
			ft_error(ast, "redirection");
		res = dup2(fd, STDIN_FILENO);
		if (res < 0)
		{
			close(fd);
			ft_error(ast, "redirection");
		}
	}
	else if (ast->type == N_GREAT)
	{
		fd = open(ast->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			ft_error(ast, "redirection");
		res = dup2(fd, STDOUT_FILENO);
		if (res < 0)
		{
			close(fd);
			ft_error(ast, "redirection");
		}
	}
	else
	{
		fd = open(ast->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
			ft_error(ast, "redirection");
		res = dup2(fd, STDOUT_FILENO);
		if (res < 0)
		{
			close(fd);
			ft_error(ast, "redirection");
		}
	}
	if (fd >= 0)
		close (fd);
	return (0);
}

int scan_for_heredoc_recursive(t_ast *ast)
{
	int left_result;
	int right_result;

    if (ast == NULL)
        return 0;
    if (ast->type == N_DLESS)
        return 1;
    left_result = scan_for_heredoc_recursive(ast->left);
    right_result = scan_for_heredoc_recursive(ast->right);
    if (left_result || right_result)
        return 1;
   	else
        return 0;
}

int scan_for_heredoc(t_ast *ast)
{
    return scan_for_heredoc_recursive(ast);
}


void pipe_execution(t_ast *ast)
{
	int pipefd[2];
	pid_t pid1;
	pid_t pid2;

	if (scan_for_heredoc(ast->left))
		return (evaluate_ast(ast->left));
	else if (scan_for_heredoc(ast->right))
		return (evaluate_ast(ast->right));
	if (pipe(pipefd) == -1)
		ft_error(ast, "pipe");
	pid1 = fork();
	if (pid1 == -1)
		ft_error(ast, "fork");
	if (pid1 == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		evaluate_ast(ast->left);
		exit(0);
	}
	pid2 = fork();
	if (pid2 == -1)
		ft_error(ast, "fork");
	if (pid2 == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		evaluate_ast(ast->right);
		exit(0);
	}
	close (pipefd[0]);
	close (pipefd[1]);
	if (waitpid(pid1, NULL, 0) == -1 || waitpid(pid2, NULL, 0) == -1)
		ft_error(ast, "waitpid");
}

void and_or_execution(t_ast *ast)
{
	int status;
	pid_t pid1;
	pid_t pid2;
	int side;

	side = 0;
	pid1 = fork();
	if (pid1 == -1)
		ft_error(ast, "fork");
	if (scan_for_heredoc(ast->right))
		side++;
	if (pid1 == 0)
	{
		if (scan_for_heredoc(ast->right))
			evaluate_ast(ast->right);
		else
			evaluate_ast(ast->left);
		exit(0);
	}
	else
	{
		if (waitpid(pid1, &status, 0) == -1)
			ft_error(ast, "waitpid");
		if ((status == 0 && ast->type == N_AND) || (status != 0 && ast->type == N_OR))
		{
			pid2 = fork();
			if (pid2 == -1)
				ft_error(ast, "fork");
			if (pid2 == 0)
			{
				if (side != 0)
					evaluate_ast(ast->left);
				else
					evaluate_ast(ast->right);
				exit(0);
			}
			else
				if (waitpid(pid2, NULL, 0) == -1)
					ft_error(ast, "waitpid");
		}
	}
}

void	evaluate_ast(t_ast *ast)
{
	int	std_in;
	int	std_out;

	if (ast->type == N_LESS || ast->type == N_GREAT || ast->type == N_DGREAT || ast->type == N_DLESS)
	{
		std_in = dup(STDIN_FILENO);
		std_out = dup(STDOUT_FILENO);
		if (std_in == -1 || std_out == -1)
			ft_error(ast, "dup");
		if (redirect(ast) == 0)
			evaluate_ast(ast->left);
		if (access("heredoc_buffer", F_OK) != -1)
			unlink("heredoc_buffer");
		dup2(std_in, STDIN_FILENO);
		dup2(std_out, STDOUT_FILENO);
		close(std_in);
		close(std_out);
	}
	if (ast->type == N_PIPE)
		pipe_execution(ast);
	if (ast->type == N_AND || ast->type == N_OR)
		and_or_execution(ast);
	if (ast->type == N_COMMAND)
		command_execute(ast);
}
