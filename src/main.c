/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:54:07 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/13 00:12:02 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	leaks(void)
{
	system("leaks minishell");
}

int	main(void)
{
	t_ast	*ast;
	t_token	*token;
	t_token	*tmp;
	char	*input;
	char	*prompt;

	// atexit(leaks);
	while (1)
	{
		prompt = get_prompt();
		input = readline(prompt);
		add_history(input);
		token = get_token(input, prompt);
		tmp = token;
		print_token(tmp);
		ast = parse(&token, input, prompt);
		printf("\n\n");
		print_ast(ast);
		evaluate_ast(ast);
		free_all(ast);
	}
}
