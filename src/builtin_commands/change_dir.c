/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_dir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 16:12:28 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/10 18:33:58 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strcutoff_front(const char *str, char c)
{
	int		i;
	char	*output;
	int		j;

	i = 0;
	while (str[i] != '\0' && str[i] != c)
		i++;
	if (str[i] == c)
		i++;
	output = malloc(sizeof(char) * (ft_strlen(str) - i + 1));
	j = 0;
	while (str[i + j] != '\0')
	{
		output[j] = str[i + j];
		j++;
	}
	output[j] = '\0';
	return (output);
}

void	change_pwd(t_ast *ast)
{
	char	*pwd;
	char	*exppwd;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	pwd = ft_strjoin("PWD=", cwd);
	free(cwd);
	exppwd = export_line(pwd);
	if (variable_exists(ast->ms.exp, "declare -x PWD") != -1)
	{
		if (variable_exists(ast->ms.env, "PWD") != -1)
			ft_change_existing(pwd, ast->ms.env);
		else
			ft_add_var(pwd, &ast->ms.env);
		ft_change_existing(exppwd, ast->ms.exp);
	}
	free(pwd);
	free(exppwd);
}

void	change_oldpwd(t_ast *ast)
{
	char	*pwd;
	char	*exppwd;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	pwd = ft_strjoin("OLDPWD=", cwd);
	free(cwd);
	exppwd = export_line(pwd);
	if (variable_exists(ast->ms.exp, "declare -x OLDPWD") != -1)
	{
		if (variable_exists(ast->ms.env, "OLDPWD") != -1)
			ft_change_existing(pwd, ast->ms.env);
		else
			ft_add_var(pwd, &ast->ms.env);
		ft_change_existing(exppwd, ast->ms.exp);
	}
	free(pwd);
	free(exppwd);
}

void	back_to_oldpwd(t_ast *ast)
{
	char	*path;

	if (variable_exists(ast->ms.env, "OLDPWD") == -1)
		return ((void)ft_printf("cd: OLDPWD is not set\n"));
	path = ft_strcutoff_front(ast->ms.env[variable_exists(ast->ms.env,
				"OLDPWD")], '=');
	change_oldpwd(ast);
	chdir(path);
	free(path);
	return ;
}

void	ch_dir(t_ast *ast)
{
	char	*path;

	if (ast->args[1] != NULL && ft_strcmp(ast->args[1], "-") == 0)
		return (back_to_oldpwd(ast));
	change_oldpwd(ast);
	if (ast->args[1] == NULL || ft_strcmp(ast->args[1], "~") == 0)
	{
		if (variable_exists(ast->ms.env, "HOME") == -1)
			return ((void)ft_printf("cd: HOME is not set\n"));
		path = ft_strcutoff_front(ast->ms.env[variable_exists(ast->ms.env,
					"HOME")], '=');
		chdir(path);
		free(path);
		change_pwd(ast);
		return ;
	}
	if (chdir(ast->args[1]) == -1)
	{
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(ast->args[1], 2);
		return ((void)(ast->ms.exit_code = 1));
	}
	change_pwd(ast);
}
