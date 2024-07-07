#include "../../includes/minishell.h"
#include <dirent.h>

char	**custom_realloc(char **ptr, size_t old_size, size_t new_size)
{
	char	**new_ptr;

	if (ptr == NULL)
	{
		new_ptr = malloc(new_size * sizeof(char *));
		if (!new_ptr)
			return (NULL);
		return (new_ptr);
	}
	if (new_size <= old_size)
		return (ptr);
	new_ptr = malloc(new_size * sizeof(char *));
	if (!new_ptr)
	{
		free_double_array_char(ptr);
		return (NULL);
	}
	ft_memcpy(new_ptr, ptr, old_size * sizeof(char *));
	free(ptr);
	return (new_ptr);
}

int	match_pattern(const char *pattern, const char *str)
{
	if (str[0] == '.')
		return (0);
	if (*pattern == '\0' && *str == '\0')
		return (1);
	if (*pattern == '*' && *(pattern + 1) == '\0')
		return (1);
	if (*pattern == '*' && *(pattern + 1) != '\0')
	{
		while (*str)
		{
			if (match_pattern(pattern + 1, str))
				return (1);
			str++;
		}
		return (0);
	}
	if (*pattern == *str)
		return (match_pattern(pattern + 1, str + 1));
	return (0);
}

char	**search_with_wildcards(char *str)
{
	char			**res;
	DIR				*dir;
	struct dirent	*entry;
	int				k;

	res = NULL;
	dir = opendir(".");
	if (!dir)
		return (NULL);
	k = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (match_pattern(str, entry->d_name))
		{
			res = custom_realloc(res, k, (k + 1) * sizeof(char *));
			res[k++] = ft_strdup(entry->d_name);
		}
		entry = readdir(dir);
	}
	res = custom_realloc(res, k, (k + 1) * sizeof(char *));
	res[k] = NULL;
	if (k > 0)
		bubble_sort(res, k);
	return (closedir(dir), res);
}

char	*join_together(char **array)
{
	char	*res;
	int		i;
	char	*tmp;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = 0;
	while (array[i])
	{
		if (i != 0)
		{
			tmp = res;
			res = ft_strjoin(tmp, " ");
			free(tmp);
			if (!res)
				return (NULL);
		}
		res = ft_str_join_free(res, array[i]);
		if (!res)
			return (NULL);
		i++;
	}
	res = remove_char(res, '*');
	return (free(array), res);
}

char	*handle_wildcards(char *str)
{
	char	**array;
	char	*res;

	array = NULL;
	res = NULL;
	array = search_with_wildcards(str);
	if (!array)
		return (NULL);
	if (!array[0])
		return (free(array[0]), NULL);
	res = join_together(array);
	if (!(res))
		return (NULL);
	return (res);
}
