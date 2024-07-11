/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_and_or.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tecker <tecker@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:42:18 by tecker            #+#    #+#             */
/*   Updated: 2024/07/11 14:14:02 by tecker           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	scan_for_heredoc_recursive(t_ast *ast)
{
	int	left_result;
	int	right_result;

	if (ast == NULL)
		return (0);
	if (ast->type == N_DLESS)
		return (1);
	left_result = scan_for_heredoc_recursive(ast->left);
	right_result = scan_for_heredoc_recursive(ast->right);
	if (left_result || right_result)
		return (1);
	else
		return (0);
}

int	scan_for_heredoc(t_ast *ast)
{
	return (scan_for_heredoc_recursive(ast));
}

int	and_or_execution(t_ast *ast)
{
	int		status;
	pid_t	pid1;
	pid_t	pid2;
	int		side;

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
		if (WIFEXITED(status) && ((WEXITSTATUS(status) == 0
					&& ast->type == N_AND) || (WEXITSTATUS(status) != 0
					&& ast->type == N_OR)))
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
