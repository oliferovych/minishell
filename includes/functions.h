/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:55:24 by dolifero          #+#    #+#             */
/*   Updated: 2024/06/25 14:38:27 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNCTIONS_H
# define FUNCTIONS_H

# include "minishell.h"
//MINISHELL PROMPT
char			*get_prompt(void);
char			*trim_dir_path(char *dir_path);

//TOKENISING
t_token			*get_token(char *input, char *prompt);
void			print_token(t_token *token);
void			free_tokens(t_token *token);
int				ft_isspace(int c);

//PARSING
t_ast			*parse(t_token **token, t_data *old_data);
void			print_ast(t_ast *node);
void			free_ast(t_ast *node);
int				error_indicator(int i, char *str);
t_ast			*expr(int prec, t_token **token, t_data *data);
char			*strcutoff_front(const char *str, char c);

//EXECUTION
void			evaluate_ast(t_ast *ast);
void			command_execute(t_ast *ast);
int				command_is_builtin(char **args);
void			ft_execute_builtin(t_ast *ast, int builtin);
void			ft_execvp(t_ast *ast);
void			ft_errorcode_exit(char *message, int code, t_ast *ast);

//ENVIRONMENT
char			**env_init(char **input_env);
char			**exp_init(char **environment);
void			print_env(char **environment);
void			print_exp(char **exp);
void			ft_del_var(int i, char **env);
void			ft_add_var(char *var, char ***env);
int				variable_exists2(char **env, char *var);
void			ft_change_existing(char *var, char **env);
int				variable_exists(char **env, char *var);

//BUILTIN COMMANDS
void			ch_dir(t_ast *ast);
void			ft_pwd(void);
void			ft_echo(t_ast *ast);
void			ft_exit(t_ast *ast);
void			ft_env(t_ast *ast);
void			ft_unset(t_ast *ast);
void			ft_exp(t_ast *ast);

//UTILITIES
void			free_all(t_ast *ast, int flag);
void			ft_error(t_ast *ast, char *str);
int				compare_till_in_1(char *str1, char *str2, char c);
int				compare_till(char *str1, char *str2, char c);
int				check_filename(t_ast *ast);
char			*ft_strcutoff_front(const char *str, char c);
void			ft_swap(char **a, char **b);
void			bubble_sort(char **array, int size);

#endif
