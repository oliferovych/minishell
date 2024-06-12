#include "../../includes/minishell.h"

const char	*node_type_to_string(t_node_type type)
{
	switch (type)
	{
	case N_COMMAND:
		return ("N_COMMAND");
	case N_LESS:
		return ("N_LESS");
	case N_GREAT:
		return ("N_GREAT");
	case N_DLESS:
		return ("N_DLESS");
	case N_DGREAT:
		return ("N_DGREAT");
	case N_PIPE:
		return ("N_PIPE");
	case N_AND:
		return ("N_AND");
	case N_OR:
		return ("N_OR");
	default:
		return ("UNKNOWN");
	}
}
#include <string.h>

void	print_ast_helper(t_ast *node, char *prefix)
{
	if (!node)
		return ;
	printf("%sNode Type: %s\n", prefix, node_type_to_string(node->type));
	if (node->filename)
	{
		printf("%sFilename: %s\n", prefix, node->filename);
	}
	if (node->heredoc)
	{
		printf("%sHeredoc: %s\n", prefix, node->heredoc);
	}
	if (node->args)
	{
		printf("%sArguments: ", prefix);
		for (int i = 0; node->args[i]; i++)
		{
			printf("%s ", node->args[i]);
		}
		printf("\n");
	}
	if (node->left)
	{
		printf("%sLeft:\n", prefix);
		print_ast_helper(node->left, strcat(strcat(strdup(prefix), "  "),
				"  "));
	}
	if (node->right)
	{
		printf("%sRight:\n", prefix);
		print_ast_helper(node->right, strcat(strcat(strdup(prefix), "  "),
				"  "));
	}
}

void	print_ast(t_ast *node)
{
	print_ast_helper(node, "");
	printf("\n");
}

const char	*token_type_to_string(t_token_type type)
{
	if (type == T_IDENTIFIER)
		return ("T_IDENTIFIER");
	else if (type == T_PIPE)
		return ("T_PIPE	");
	else if (type == T_GREAT)
		return ("T_GREAT	");
	else if (type == T_LESS)
		return ("T_LESS	");
	else if (type == T_DGREAT)
		return ("T_DGREAT");
	else if (type == T_DLESS)
		return ("T_DLESS	");
	else if (type == T_OR)
		return ("T_OR	");
	else if (type == T_AND)
		return ("T_AND	");
	else if (type == T_OPAR)
		return ("T_OPAR	");
	else if (type == T_CPAR)
		return ("T_CPAR	");
	else
		return ("UNKNOWN");
}

void	print_token(t_token *token)
{
	int		i;
	t_token	*head;

	i = 1;
	head = token;
	while (head)
	{
		printf("\n");
		printf("token %d, type:%s		value: %s", i,
					token_type_to_string(head->type), head->value);
		i++;
		head = head->next;
	}
}

void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	if (node->type == N_COMMAND)
	{
		if (node->args)
		{
			for (int i = 0; node->args[i]; i++)
			{
				free(node->args[i]);
			}
			free(node->args);
		}
	}
	else if (node->type == N_GREAT || node->type == N_LESS)
	{
		if (node->filename)
			free(node->filename);
	}
	free_ast(node->left);
	free_ast(node->right);
	free(node);
}

void	free_tokens(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token;
		token = token->next;
		free(tmp->value);
		tmp->value = NULL;
		free(tmp);
	}
}

int	error_indicator(int i, char *str)
{
	static int	b = 0;

	if (str)
		ft_printf("%s\n", str);
	if (i == 1)
		b++;
	if (i == 3)
		b = 0;
	return (b);
}

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t'
		|| c == '\v');
}
