/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tecker <tecker@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:54:07 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/09 16:50:30 by tecker           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	leaks(void)
{
	system("leaks minishell");
}

static t_data	*data_init(char **envp)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		exit(EXIT_FAILURE);
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

static void	get_input(t_data *data, t_ast *ast)
{
	data->prompt = get_prompt();
	data->input = readline(data->prompt);
	if (!data->input)
	{
		ft_printf("%sexit\n", data->prompt);
		if (ast != NULL)
			free_all(ast, 1);
		else
		{
			free(data->prompt);
			free(data->input);
			free_environment(data->env);
			free_environment(data->exp);
			free(data);
		}
		exit(0);
	}
	add_history(data->input);
}

int	main(int argc, char **argv, char **envp)
{
	t_ast	*ast;
	t_token	*token;
	// t_token	*tmp;
	t_data	*data;

	(void)argc;
	(void)argv;
	ft_initialize_signals();
	data = data_init(envp);
	ast = ft_get_ast();
	ast = NULL;
	while (1)
	{
		get_input(data, ast);
		token = get_token(data->input, data->prompt);
		// tmp = token;
		// print_token(tmp);
		write(1, "\naa\n", 4);
		ast = parse(&token, data);
		write(1, "\nbb\n", 4);
		// printf("\n\n");
		// print_ast(ast);
		evaluate_ast(ast);
		data->env = ast->ms.env;
		data->exp = ast->ms.exp;
		free_all(ast, 0);
	}
}
