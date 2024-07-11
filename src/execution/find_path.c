/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:10:15 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/11 20:57:37 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_errorcode_exit(char *command, char *path)
{
	ft_putstr_fd(command, 2);
	if (ft_strcmp(command, ".") == 0)
	{
		ft_putendl_fd(": filename argument required", 2);
		ft_putendl_fd(".: usage: . filename [arguments]", STDERR_FILENO);
		free(path);
		exit(2);
	}
	if (ft_strcmp(path, command) == 0 && !(ft_strncmp(command, "./",
				2) == 0 || ft_strncmp(command, "/", 1) == 0
			|| ft_strncmp(command, "../", 3) == 0))
		ft_putendl_fd(": command not found", 2);
	else
	{
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
	}
	free(path);
	if (errno == EACCES || errno == EISDIR)
		exit(126);
	exit(127);
}

static char	*ft_find_path(t_ast *ast)
{
	char	*path;

	if (ft_strncmp(ast->args[0], "./", 2) == 0 || ft_strncmp(ast->args[0], "/",
			1) == 0 || ft_strncmp(ast->args[0], "../", 3) == 0
		|| access(ast->args[0], F_OK) == 0)
		path = ft_strdup(ast->args[0]);
	else
	{
		if (variable_exists(ast->ms.env, "PATH") != -1)
			path = ft_strdup(ast->ms.env[variable_exists(ast->ms.env, "PATH")]
					+ 5);
		else
			path = ft_strdup(ast->args[0]);
	}
	return (path);
}

static char	*ft_get_full_path(char *spl_path, t_ast *ast)
{
	char	*full_path;

	full_path = malloc(ft_strlen(spl_path) + ft_strlen(ast->args[0])
			+ 2);
	ft_strlcpy(full_path, spl_path, ft_strlen(spl_path)
		+ ft_strlen(ast->args[0]) + 2);
	ft_strlcat(full_path, "/", ft_strlen(spl_path)
		+ ft_strlen(ast->args[0]) + 2);
	ft_strlcat(full_path, ast->args[0], ft_strlen(spl_path)
		+ ft_strlen(ast->args[0]) + 2);
	return (full_path);
}

static void	case_split_path(char *path, t_ast *ast)
{
	char	**spl_path;
	char	*full_path;
	int		i;

	spl_path = ft_split(path, ':');
	free(path);
	i = 0;
	while (spl_path[i] != NULL)
	{
		full_path = ft_get_full_path(spl_path[i], ast);
		if (execve(full_path, ast->args, ast->ms.env) == -1)
		{
			if (spl_path[i + 1] == NULL)
			{
				full_path = ft_strdup(ast->args[0]);
				ft_errorcode_exit(ast->args[0], full_path);
			}
			free(full_path);
		}
		i++;
	}
}

void	ft_execvp(t_ast *ast)
{
	char	*path;

	path = ft_find_path(ast);
	if (ft_strchr(path, ':') && variable_exists(ast->ms.env, "PATH") >= 0)
		case_split_path(path, ast);
	else
	{
		if (execve(path, ast->args, ast->ms.env) == -1)
			ft_errorcode_exit(ast->args[0], path);
		free(path);
	}
	exit(0);
}
