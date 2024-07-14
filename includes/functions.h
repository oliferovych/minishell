/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomecker <tomecker@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:55:24 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/14 09:49:54 by tomecker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNCTIONS_H
# define FUNCTIONS_H

# include "minishell.h"
//MINISHELL PROMPT
char			*get_prompt(void);
char			*trim_dir_path(char *dir_path);

//TOKENISING
t_token			*get_token(char *input);
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
void			link_redir_nodes(t_ast **redir_node, t_ast **command_node);
int				add_token(t_token **lst, t_token_type type,
					char *input, int *i);
char			*process_value(char *input, int *i);
int				compare_quotes(int *quotes, char *val, int *remove);
int				count_single_quotes(char *str);
char			*transform_argiables(char *str, int *i, t_ast **node);
char			*transform_variable(char *line, t_ast *ast);
void			double_single_quotes(char **val);
char			*set_flag(char *str, int *flag);
void			check_for_var(char **res, int flag);
void			bb(const char *str, int *remove);
int				aa(char **val);
void			add_double_single_quotes(char **val);

//EXECUTION
int				evaluate_ast(t_ast *ast, int flag);
int				command_execute(t_ast *ast);
int				command_is_builtin(char **args);
void			ft_execute_builtin(t_ast *ast, int builtin);
void			ft_execvp(t_ast *ast);
void			ft_errorcode_exit(char *command, char *path);
int				get_status(int status);
int				and_or_execution(t_ast *ast);
int				pipe_execution(t_ast *ast);
int				heredoc(int *fd, t_ast *ast, int flag);
int				redirect(t_ast *ast, int flag);

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
int				ft_error(t_ast *ast, char *str);
int				compare_till_in_1(char *str1, char *str2, char c);
int				compare_till(char *str1, char *str2, char c);
int				check_filename(t_ast *ast);
char			*ft_strcutoff_front(const char *str, char c);
void			ft_swap(char **a, char **b);
void			bubble_sort(char **array, int size);
t_ast			*ft_get_ast(void);
char			*remove_char(char *str, char char_to_remove);
void			free_double_array_char(char **array);
void			free_command(t_ast *node, int i);
int				check_string(const char *str);

//WILDCARDS
char			*handle_wildcards(char *str);
char			*ft_str_join_free(char *s1, char *s2);

#endif
