/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:38:52 by tecker            #+#    #+#             */
/*   Updated: 2024/07/11 14:37:20 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	out(int *fd, t_ast *ast)
{
	int	res;

	*fd = open(ast->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd < 0)
		return (ft_error(ast, "redirect failed"));
	res = dup2(*fd, STDOUT_FILENO);
	if (res < 0)
	{
		close(*fd);
		return (ft_error(ast, "redirect failed"));
	}
	return (0);
}

int	in(int *fd, t_ast *ast)
{
	int	res;

	*fd = open(ast->filename, O_RDONLY);
	if (fd < 0)
		return (ft_error(ast, "redirect failed"));
	res = dup2(*fd, STDIN_FILENO);
	if (res < 0)
	{
		close(*fd);
		return (ft_error(ast, "redirect failed"));
	}
	return (0);
}

int	append(int *fd, t_ast *ast)
{
	int	res;

	*fd = open(ast->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (*fd < 0)
		return (ft_error(ast, "redirect failed"));
	res = dup2(*fd, STDOUT_FILENO);
	if (res < 0)
	{
		close(*fd);
		return (ft_error(ast, "redirect failed"));
	}
	return (0);
}

int	redirect(t_ast *ast)
{
	int	fd;

	fd = -1;
	if (ast->filename && check_filename(ast))
		return (ft_printf("%s: ambiguous redirect\n", ast->filename));
	if (ast->type == N_LESS)
	{
		if (in(&fd, ast))
			return (1);
	}
	else if (ast->type == N_DLESS)
	{
		if (heredoc(&fd, ast))
			return (1);
	}
	else if (ast->type == N_GREAT && out(&fd, ast))
		return (1);
	else
	{
		if (append(&fd, ast))
			return (1);
	}
	if (fd >= 0)
		close (fd);
	return (0);
}
