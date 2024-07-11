/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:36:08 by tecker            #+#    #+#             */
/*   Updated: 2024/07/11 19:20:50 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_redirection(t_ast *ast)
{
	if (ast->left->type == N_LESS || ast->left->type == N_DLESS
		|| ast->left->type == N_GREAT || ast->left->type == N_DGREAT)
	{
		if (redirect(ast->left, 0) != 0)
			return (ft_printf("piping failed\n"), 1);
	}
	return (0);
}

int	create_child_process(pid_t *pid)
{
	*pid = fork();
	if (*pid == -1)
		return (ft_printf("piping failed\n"), 1);
	return (0);
}

void	execute_left_child(t_ast *ast, int pipefd[2])
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

void	execute_right_child(t_ast *ast, int pipefd[2])
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	evaluate_ast(ast->right, 0);
	exit(0);
}

int	pipe_execution(t_ast *ast)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(pipefd) == -1)
		return (ft_printf("piping failed\n"), 1);
	if (handle_redirection(ast))
		return (1);
	if (create_child_process(&pid1))
		return (1);
	if (pid1 == 0)
		execute_left_child(ast, pipefd);
	if (create_child_process(&pid2))
		return (1);
	if (pid2 == 0)
		execute_right_child(ast, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	if (waitpid(pid1, NULL, 0) == -1 || waitpid(pid2, NULL, 0) == -1)
		return (ft_printf("piping failed\n"), 1);
	return (0);
}
