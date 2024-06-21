#include "../../includes/minishell.h"

void	ft_errorcode_exit(char *message, int code, t_ast *ast)
{
	char *var;
	char *char_code;

	char_code = ft_itoa(code);
	var = ft_strjoin("?=", char_code);
	ft_printf("%s\n", var);
	free(char_code);
	if (message != NULL)
		perror(ast->args[0]);
	ft_change_existing(var, ast->ms.env);
	free(var);
	exit(code);
}

void	ft_execvp(t_ast *ast)
{
	char *path;
	char **spl_path;
	char *full_path;
	int i;

	if (variable_exists(ast->ms.env, "PATH") || variable_exists(ast->ms.env, "PATH") == 0)
	{
		path = ft_strdup(ast->ms.env[variable_exists(ast->ms.env, "PATH")] + 5);
		spl_path = ft_split(path, ':');
		free(path);
		i = 0;
		while (spl_path[i] != NULL)
		{
			full_path = malloc(strlen(spl_path[i]) + strlen(ast->args[0]) + 2);
			ft_strlcpy(full_path, spl_path[i], strlen(spl_path[i]) + strlen(ast->args[0]) + 2);
			ft_strlcat(full_path, "/", strlen(spl_path[i]) + strlen(ast->args[0]) + 2);
			ft_strlcat(full_path, ast->args[0], strlen(spl_path[i]) + strlen(ast->args[0]) + 2);
			if (execve(full_path, ast->args, ast->ms.env) == -1)
			{
				free(full_path);
				if (spl_path[i + 1] == NULL)
					ft_errorcode_exit("Command not found.", 127, ast);
			}
			i++;
		}
		ft_errorcode_exit(NULL, 0, ast);
	}
	return (ft_errorcode_exit("Path is not set.", 127, ast));
}
