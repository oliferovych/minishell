/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:54:07 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/14 23:26:28 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_data	*data_init(char **envp)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		exit(EXIT_FAILURE);
	data->exit_code = 0;
	data->env = env_init(envp);
	if (!data->env)
	{
		free(data);
		exit(EXIT_FAILURE);
	}
	data->exp = exp_init(data->env);
	if (!data->exp)
	{
		free_environment(data->env);
		free(data);
		exit(EXIT_FAILURE);
	}
	return (data);
}

static void	get_input(t_data *data)
{
	data->prompt = get_prompt();
	data->input = readline(data->prompt);
	if (!data->input)
	{
		ft_printf("%sexit\n", data->prompt);
		free(data->prompt);
		free(data->input);
		free_environment(data->env);
		free_environment(data->exp);
		free(data);
		exit(1);
	}
	add_history(data->input);
}

static void	free_data(t_data *data, t_token *token)
{
	free(data->prompt);
	free(data->input);
	free_tokens(token);
}

int	main(int argc, char **argv, char **envp)
{
	t_ast	*ast;
	t_token	*token;
	t_data	*data;

	(void)argc;
	(void)argv;
	ft_initialize_signals();
	data = data_init(envp);
	ast = ft_get_ast();
	while (1)
	{
		ast = NULL;
		get_input(data);
		token = get_token(data->input);
		ast = parse(token, data);
		if (!ast)
			free_data(data, token);
		else if (!evaluate_ast(ast, 1))
		{
			data->env = ast->ms.env;
			data->exp = ast->ms.exp;
			data->exit_code = ast->ms.exit_code;
		}
		free_all(ast, 0);
	}
}
