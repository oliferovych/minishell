#include "../../includes/minishell.h"
#include <dirent.h>

static int match_segments(const char *str, char **segments)
{
    const char *s = str;
    int i = 0;

    while (segments[i] != NULL)
	{
        s = strstr(s, segments[i]);
        if (!s) return 0;
        s += strlen(segments[i]);
        i++;
    }
    return 1;
}

void free_double_array_char(char **array)
{
	int i;
	
	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

char **ft_split_inc_char(char *str, char c)
{
	char **res;
	int i;
	int j;

	i = 0;
	j = 0;
	while(str[i])
	{
		
	}
}

char **search_with_wildcards(char *str)
{
	char **res;
    DIR *dir;
    struct dirent *entry;
    char **segments;
	int k;

	res = NULL;
    segments = ft_split_inc_char(str, '*');
    if (!segments)
		return (NULL);
    dir = opendir(".");
    if (!dir)
	{
        free_double_array_char(segments);
        return (NULL);
    }
	k = 0;
    while ((entry = readdir(dir)) != NULL)
	{
        if (match_segments(entry->d_name, segments))
		{
			res = realloc(res, (k + 1) * sizeof(char *));
			res[k] = ft_strdup(entry->d_name);
			k++;
        }
    }
	res = realloc(res, (k + 1) * sizeof(char *));
	res[k] = NULL;
    closedir(dir);
    free_double_array_char(segments);
	return(res);
}

char *join_together(char **array)
{
    char *res;
    int i;
	char *tmp;

	res = ft_strdup("");
	if (!res)
        return NULL;
  	i = 0;
    while (array[i])
	{
        if (i != 0)
		{
            tmp = res;
            res = ft_strjoin(tmp, " ");
            free(tmp);
            if (!res)
                return NULL;
        }
        res = ft_str_join_free(res, array[i]);
		if (!res)
			return NULL;
        i++;
    }
	res = remove_char(res, '*');
    return (res);
}

void handle_wildcards(char *str, char **res)
{
	char **array;
	
    array = search_with_wildcards(str);
	if (!array)
		error_indicator(1, "wildcard parsing failed");
	*res = join_together(array);
	if (!(*res))
		error_indicator(1, "wildcard parsing failed");
}