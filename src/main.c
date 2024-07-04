/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:54:07 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/28 12:37:47 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	leaks(void)
{
	system("leaks minishell");
}

int	main(int argc, char **argv, char **envp)
{
	t_ast	*ast;
	t_token	*token;
	t_token	*tmp;
	t_data	*data;

	(void)argc;
	(void)argv;
	data = malloc(sizeof(t_data));
	data->env = env_init(envp);
	data->exp = exp_init(data->env);
	if (!data->env || !data->exp)
		exit(1);
	while (1)
	{
		data->prompt = get_prompt();
		data->input = readline(data->prompt);
		add_history(data->input);
		token = get_token(data->input, data->prompt);
		tmp = token;
		print_token(tmp);
		ast = parse(&token, data);
		printf("\n\n");
		print_ast(ast);
		evaluate_ast(ast);
		data->env = ast->ms.env;
		data->exp = ast->ms.exp;
		free_all(ast, 0);
	}
}
