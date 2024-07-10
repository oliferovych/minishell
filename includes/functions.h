/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:55:24 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/11 00:34:17 by tomecker         ###   ########.fr       */
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
void			ft_token_list_add_back(t_token **lst, t_token *new_token);
char			*transform_arg(t_ast **node, t_token *token, int k);
t_ast			*create_ast_node(t_node_type type, t_data *data);
void			create_node(t_token_type type, t_ast **node, t_data *data);
char			*create_redir_node(t_token **token, t_ast **redir_node);
char			*create_command_node(t_token **token,
					t_ast **node, t_data *data);
void			handle_r(t_token **token, t_data *data,
					t_ast **node, t_ast **prev_node);
void			handle_c(t_token **token, t_data *data,
					t_ast **node, t_ast **prev_redi);
int				is_redirection(t_token_type type);

//EXECUTION
int				evaluate_ast(t_ast *ast, int flag);
int				command_execute(t_ast *ast);
int				command_is_builtin(char **args);
void			ft_execute_builtin(t_ast *ast, int builtin);
void			ft_execvp(t_ast *ast);
void			ft_errorcode_exit(char *command, int code);

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
char			*export_line(char *env_line);

//BUILTIN COMMANDS
void			ch_dir(t_ast *ast);
void			ft_pwd(void);
void			ft_echo(t_ast *ast);
void			ft_exit(t_ast *ast);
void			ft_env(t_ast *ast);
void			ft_unset(t_ast *ast);
void			ft_exp(t_ast *ast);

//SIGNALS
void			shell_handler_sigint(int signum);
void			heredoc_handler_sigint(int signum);
void			ft_sigmode_shell(void);
void			ft_sigmode_heredoc(void);
void			ft_initialize_signals(void);

//UTILITIES
void			free_all(t_ast *ast, int flag);
void			free_environment(char **environment);
void			ft_error(t_ast *ast, char *str);
int				compare_till_in_1(char *str1, char *str2, char c);
int				compare_till(char *str1, char *str2, char c);
int				check_filename(t_ast *ast);
char			*ft_strcutoff_front(const char *str, char c);
void			ft_swap(char **a, char **b);
void			bubble_sort(char **array, int size);
t_ast			*ft_get_ast(void);
char			*remove_char(const char *str, char char_to_remove);
void			free_double_array_char(char **array);

//WILDCARDS
char			*handle_wildcards(char *str);
char			*ft_str_join_free(char *s1, char *s2);

#endif
