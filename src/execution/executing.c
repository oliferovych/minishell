/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:09:27 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/11 00:34:00 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	command_execute(t_ast *ast)
{
	int	builtin;
	int	pid;

	builtin = command_is_builtin(ast->args);
	if (builtin)
		ft_execute_builtin(ast, builtin);
	else
	{
		pid = fork();
		if (pid == -1)
			return (ft_printf("command execution failed\n"), 1);
		if (pid == 0)
		  ft_execvp(ast);
		else
			if (waitpid(pid, NULL, 0) == -1)
				return (ft_printf("command execution failed\n"), 1);
	}
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

int and_or_execution(t_ast *ast)
{
	int status;
	pid_t pid1;
	pid_t pid2;
	int side;

	side = 0;
	pid1 = fork();
	if (pid1 == -1)
		return (ft_printf("and/or failed\n"), 1);
	if (scan_for_heredoc(ast->right))
		side++;
	if (pid1 == 0)
	{
		if (scan_for_heredoc(ast->right))
			evaluate_ast(ast->right, 0);
		else
			evaluate_ast(ast->left, 0);
		exit(0);
	}
	else
	{
		if (waitpid(pid1, &status, 0) == -1)
			return (ft_printf("and/or failed\n"), 1);
		if (WIFEXITED(status) && ((WEXITSTATUS(status) == 0 && ast->type == N_AND) || (WEXITSTATUS(status) != 0 && ast->type == N_OR)))
		{
			pid2 = fork();
			if (pid2 == -1)
				return (ft_printf("and/or failed\n"), 1);
			if (pid2 == 0)
			{
				if (side != 0)
					evaluate_ast(ast->left, 0);
				else
					evaluate_ast(ast->right, 0);
				exit(0);
			}
			else
				if (waitpid(pid2, NULL, 0) == -1)
					return (ft_printf("and/or failed\n"), 1);
		}
	}
	return (0);
}

int redirect(t_ast *ast)
{
	int fd;
	int fd2;
	int res;
	char *line;
	int reset_stdin;

	if (ast->filename && check_filename(ast))
		return (ft_printf("%s: ambiguous redirect\n", ast->filename));
	if (ast->type == N_LESS)
	{
		fd = open(ast->filename, O_RDONLY);
		if (fd < 0)
			return(ft_printf("redirect failed\n"), 1);
		res = dup2(fd, STDIN_FILENO);
		if (res < 0)
		{
			close(fd);
			return (ft_printf("redirect failed\n"), 1);
		}
	}
	else if (ast->type == N_DLESS)
	{
		reset_stdin = open("/dev/tty", O_RDONLY);
        if (reset_stdin < 0)
			return (ft_printf("redirect failed\n"), 1);
        dup2(reset_stdin, STDIN_FILENO);
        close(reset_stdin);
		ft_sigmode_heredoc();
		fd2 = open("heredoc_buffer", O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0644);
		if (fd2 < 0)
			return (ft_printf("redirect failed\n"), 1);
		while (1)
		{
			line = readline("> ");
			if (ft_strlen(line) == ft_strlen(ast->heredoc) &&
            	ft_strncmp(line, ast->heredoc, ft_strlen(ast->heredoc)) == 0)
			{
				if (line)
					free(line);
				break;
			}
			else
			{
				write(fd2, line, ft_strlen(line));
				write(fd2, "\n", 1);
				if (line)
					free(line);
			}
		}
		close(fd2);
		fd = open("heredoc_buffer", O_RDONLY);
		if (fd < 0)
			return (ft_printf("redirect failed\n"), 1);
		res = dup2(fd, STDIN_FILENO);
		if (res < 0)
		{
			close(fd);
			return (ft_printf("redirect failed\n"), 1);
		}
		ft_sigmode_shell();
	}
	else if (ast->type == N_GREAT)
	{
		fd = open(ast->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			return (ft_printf("redirect failed\n"), 1);
		res = dup2(fd, STDOUT_FILENO);
		if (res < 0)
		{
			close(fd);
			return (ft_printf("redirect failed\n"), 1);
		}
	}
	else
	{
		fd = open(ast->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
			return (ft_printf("redirect failed\n"), 1);

		res = dup2(fd, STDOUT_FILENO);
		if (res < 0)
		{
			close(fd);
			return (ft_printf("redirect failed\n"), 1);
		}
	}
	if (fd >= 0)
		close (fd);
	return (0);
}

int	pipe_execution(t_ast *ast)
{
    int pipefd[2];
    pid_t pid1;
    pid_t pid2;

    if (pipe(pipefd) == -1)
       	return (ft_printf("piping failed\n"), 1);
	if (ast->left->type == N_LESS || ast->left->type == N_DLESS || ast->left->type == N_GREAT || ast->left->type == N_DGREAT)
	{
		if (redirect(ast->left) != 0)
			return (ft_printf("piping failed\n"), 1);
	}
    pid1 = fork();
    if (pid1 == -1)
		return (ft_printf("piping failed\n"), 1);
    if (pid1 == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        if (ast->left->type == N_DLESS)
            evaluate_ast(ast->left->left, 0);
        else
            evaluate_ast(ast->left, 0);
        exit(0);
    }
    pid2 = fork();
    if (pid2 == -1)
		return (ft_printf("piping failed\n"), 1);
    if (pid2 == 0)
    {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        evaluate_ast(ast->right, 0);
        exit(0);
    }
    close(pipefd[0]);
    close(pipefd[1]);
    if (waitpid(pid1, NULL, 0) == -1 || waitpid(pid2, NULL, 0) == -1)
		return (ft_printf("piping failed\n"), 1);
    return (0);
}

int evaluate_ast(t_ast *ast, int flag)
{
    int std_in;
    int std_out;
	if (flag)
	{
		std_in = dup(STDIN_FILENO);
		std_out = dup(STDOUT_FILENO);
		if (std_in == -1 || std_out == -1)
			return (1);
	}
	if (!ast || !ast->ms.token)
            return (1);
    if (ast->type == N_PIPE)
        pipe_execution(ast);
    else if (ast->type == N_AND || ast->type == N_OR)
        and_or_execution(ast);
    else if (ast->type == N_LESS || ast->type == N_GREAT || ast->type == N_DGREAT || ast->type == N_DLESS)
	{
		if (redirect(ast) == 0)
            evaluate_ast(ast->left, 0);
	}
    else if (ast->type == N_COMMAND)
        command_execute(ast);
	if (flag)
	{
		dup2(std_in, STDIN_FILENO);
		dup2(std_out, STDOUT_FILENO);
		close(std_in);
		close(std_out);
		if (access("heredoc_buffer", F_OK) != -1)
			unlink("heredoc_buffer");
	}
    return (0);
}

