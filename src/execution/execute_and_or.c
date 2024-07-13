/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_and_or.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:42:18 by tecker            #+#    #+#             */
/*   Updated: 2024/07/13 21:58:49 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	scan_for_heredoc(t_ast *ast)
{
	if (ast->type == N_DLESS)
		return (1);
	return (0);
}

void	handle_first_child(t_ast *ast)
{
	if (scan_for_heredoc(ast->right) && !scan_for_heredoc(ast->left))
	{
		redirect(ast->right, 0);
		if (ast->right->ms.exit_code != 0)
			exit(ast->right->ms.exit_code);
		evaluate_ast(ast->right->left, 0);
		exit(ast->right->left->ms.exit_code);
	}
	else if ((scan_for_heredoc(ast->right) && scan_for_heredoc(ast->left))
		|| (scan_for_heredoc(ast->left) && ast->right->type == N_AND))
	{
		redirect(ast->left, 1);
		exit(ast->left->ms.exit_code);
	}
	else
	{
		evaluate_ast(ast->left, 0);
		exit(ast->left->ms.exit_code);
	}
}

void	handle_secound_child(t_ast *ast)
{
	if (scan_for_heredoc(ast->right) && !scan_for_heredoc(ast->left))
	{
		evaluate_ast(ast->left, 0);
		exit(ast->left->ms.exit_code);
	}
	else if ((scan_for_heredoc(ast->right) && scan_for_heredoc(ast->left))
		|| scan_for_heredoc(ast->right))
	{
		redirect(ast->right, 1);
		if (ast->right->ms.exit_code != 0)
			exit(ast->right->ms.exit_code);
		evaluate_ast(ast->right->left, 0);
		exit(ast->right->left->ms.exit_code);
	}
	else
	{
		evaluate_ast(ast->right, 0);
		exit(ast->right->ms.exit_code);
	}
}

int	and_or_execution(t_ast *ast)
{
	int		status;
	pid_t	pid1;
	pid_t	pid2;

	pid1 = fork();
	if (pid1 == -1)
		return (ft_error(ast, "and/or failed"));
	if (pid1 == 0)
		handle_first_child(ast);
	if (waitpid(pid1, &status, 0) == -1)
		return (ft_error(ast, "and/or failed"));
	ast->ms.exit_code = WEXITSTATUS(status);
	if (WIFEXITED(status) && ((WEXITSTATUS(status) == 0 && ast->type == N_AND)
			|| (WEXITSTATUS(status) != 0 && ast->type == N_OR)))
	{
		pid2 = fork();
		if (pid2 == -1)
			return (ft_error(ast, "and/or failed"));
		if (pid2 == 0)
			handle_secound_child(ast);
		else if (waitpid(pid2, &status, 0) == -1)
			return (ft_error(ast, "and/or failed"));
		ast->ms.exit_code = WEXITSTATUS(status);
	}
	return (0);
}
