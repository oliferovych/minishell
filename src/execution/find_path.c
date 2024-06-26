#include "../../includes/minishell.h"

void	ft_errorcode_exit(char *message, int code, t_ast *ast)
{
	char	*var;
	char	*char_code;

	char_code = ft_itoa(code);
	var = ft_strjoin("?=", char_code);
	free(char_code);
	if (message != NULL)
		perror(message);
	ft_change_existing(var, ast->ms.env);
	free(var);
	ft_printf("%s\n", ast->ms.env[variable_exists(ast->ms.env, "?")]);
	exit(code);
}

char	*ft_find_path(t_ast *ast)
{
	char	*path;

	if (ft_strncmp(ast->args[0], "./", 2) == 0
		|| ft_strncmp(ast->args[0], "/", 1) == 0
		|| ft_strncmp(ast->args[0], "../", 3) == 0
		|| access(ast->args[0], F_OK) == 0)
		path = ft_strdup(ast->args[0]);
	else
		path = ft_strdup(ast->ms.env[variable_exists(ast->ms.env, "PATH")] + 5);
	return (path);
}

void	case_split_path(char *path, t_ast *ast)
{
	char	**spl_path;
	char	*full_path;
	int		i;

	spl_path = ft_split(path, ':');
	free(path);
	i = 0;
	while (spl_path[i] != NULL)
	{
		full_path = malloc(strlen(spl_path[i]) + strlen(ast->args[0]) + 2);
		ft_strlcpy(full_path, spl_path[i],
			strlen(spl_path[i]) + strlen(ast->args[0]) + 2);
		ft_strlcat(full_path, "/",
			strlen(spl_path[i]) + strlen(ast->args[0]) + 2);
		ft_strlcat(full_path, ast->args[0],
			strlen(spl_path[i]) + strlen(ast->args[0]) + 2);
		if (execve(full_path, ast->args, ast->ms.env) == -1)
		{
			free(full_path);
			if (spl_path[i + 1] == NULL)
				ft_errorcode_exit(ast->args[0], 127, ast);
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
		{
			free(path);
			ft_errorcode_exit(ast->args[0], 127, ast);
		}
	}
	ft_errorcode_exit(NULL, 0, ast);
}
