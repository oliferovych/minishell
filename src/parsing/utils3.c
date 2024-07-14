#include "../../includes/minishell.h"

void	add_double_single_quotes(char **val)
{
	char	*new_str;
	int		i;
	int		j;

	i = 0;
	j = 1;
	if (*val == NULL)
		return ;
	new_str = (char *)malloc(ft_strlen(*val) + 3);
	if (new_str == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	new_str[0] = '\'';
	while ((*val)[i])
	{
		new_str[j] = (*val)[i];
		i++;
		j++;
	}
	new_str[j] = '\'';
	new_str[j + 1] = '\0';
	free(*val);
	*val = new_str;
}

int	aa(char **val)
{
	int	i;

	if (!(*val))
		return (0);
	if ((*val)[0] == '\"' && (*val)[1] == '\'')
	{
		i = 2;
		while ((*val)[i] && (*val)[i] != '\'')
			i++;
		if ((*val)[i] == '\'' && (*val)[i + 1] == '\"')
		{
			add_double_single_quotes(val);
			return (0);
		}
	}
	return (0);
}

void	bb(const char *str, int *remove)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	if (str[i] == '\'' && str[i + 1] == '\"')
	{
		i += 2;
		while (str[i] && str[i] != '\"')
			i++;
		if (str[i] == '\"' && str[i + 1] == '\'')
		{
			*remove = 0;
			return ;
		}
	}
}
