/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_ast.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:34:47 by tecker            #+#    #+#             */
/*   Updated: 2024/07/11 21:16:46 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_std_backup(int *std_in, int *std_out)
{
	*std_in = dup(STDIN_FILENO);
	*std_out = dup(STDOUT_FILENO);
	if (*std_in == -1 || *std_out == -1)
		return (1);
	return (0);
}

void	restore_std(int std_in, int std_out)
{
	dup2(std_in, STDIN_FILENO);
	dup2(std_out, STDOUT_FILENO);
	close(std_in);
	close(std_out);
	if (access(".heredoc_buffer", F_OK) != -1)
		unlink(".heredoc_buffer");
}

int	evaluate_ast(t_ast *ast, int flag)
{
	int	std_in;
	int	std_out;

	if (flag && handle_std_backup(&std_in, &std_out))
		return (1);
	if (!ast || !ast->ms.token)
		return (1);
	if (ast->type == N_PIPE)
		pipe_execution(ast);
	else if (ast->type == N_AND || ast->type == N_OR)
		and_or_execution(ast);
	else if (ast->type == N_LESS || ast->type == N_GREAT
		|| ast->type == N_DGREAT || ast->type == N_DLESS)
	{
		if (redirect(ast, 0) == 0)
			evaluate_ast(ast->left, 0);
	}
	else if (ast->type == N_COMMAND)
		command_execute(ast);
	if (flag)
		restore_std(std_in, std_out);
	return (0);
}
