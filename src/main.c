/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:54:07 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/19 14:46:26 by tomecker         ###   ########.fr       */
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
	// t_token	*tmp;
	char	*input;
	char	*prompt;
	char	**env;
	// char	**exp;

	(void)argc;
	(void)argv;
	env = env_init(envp);
	if (!env)
		exit(1);
	// exp = exp_init(env);
	while (1)
	{
		prompt = get_prompt();
		input = readline(prompt);
		add_history(input);
		token = get_token(input, prompt);
		// tmp = token;
		// print_token(tmp);
		ast = parse(&token, input, prompt, env);
		printf("\n\n");
		print_ast(ast);
		evaluate_ast(ast);
		env = ast->ms.env;
		free_all(ast, 0);
	}
}
