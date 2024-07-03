#include "../../includes/minishell.h"

int check_filename(t_ast *ast)
{
	if (ast->filename[0] == '$' && ast->filename[1] && ast->tran[0][0] == 1)
		return(1);
	return (0);
}

char *remove_char(const char *str, char char_to_remove)
{
    int new_len;
    int i;
	int j;
	char *new_str;

	i = 0;
	new_len = 0;
    while (str[i] != '\0')
    {
        if (str[i] != char_to_remove)
            new_len++;
        i++;
    }
	new_str = (char *)malloc(new_len + 1);
    if (!new_str)
        return NULL;
    i = 0;
    j = 0;
    while (str[i] != '\0')
    {
        if (str[i] != char_to_remove)
        {
            new_str[j] = str[i];
            j++;
        }
        i++;
    }
    new_str[j] = '\0';
    return new_str;
}