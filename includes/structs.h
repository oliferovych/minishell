/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 15:08:37 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/19 14:33:43 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "minishell.h"

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

typedef struct s_shell
{
	char			*input;
	char			*prompt;
	t_token			*token;
	char			**env;
	char			**exp;
}					t_shell;

typedef struct s_data
{
	char *prompt;
	char *input;
	char **env;
	t_token *token;
}				t_data;


typedef struct s_ast
{
	t_node_type		type;
	char			**args;
	char			*filename;
	char			*heredoc;
	struct s_ast	*left;
	struct s_ast	*right;
	struct s_shell	ms;
}					t_ast;

#endif
