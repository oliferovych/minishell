#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void search_with_wildcards(const char *path, char *pattern, char **res)
{
    DIR *dir;
    struct dirent *entry;
    char **segments;

    segments = ft_split(pattern, '*');
    if (!segments) return;

    dir = opendir(path);
    if (!dir)
	{
        perror("opendir");
        int i = 0;
        while (segments[i]) free(segments[i++]);
        free(segments);
        return;
    }

    *res = strdup("");
    while ((entry = readdir(dir)) != NULL)
	{
        if (match_segments(entry->d_name, segments))
		{
            char *temp = ft_strjoin(*res, entry->d_name);
            free(*res);
            *res = temp;
        }
    }

    closedir(dir);
    int i = 0;
    while (segments[i]) free(segments[i++]);
    free(segments);
    if (*res && (*res)[0] == ' ')
        memmove(*res, *res + 1, strlen(*res));
}

void handle_wildcards(char *str, char **res) {
    char *path = ".";
    search_with_wildcards(path, str, res);
}

int main() {
    char str[] = "*l*";
    char *res;

    handle_wildcards(str, &res);
    printf("Matches: %s\n", res);
    free(res);

    return 0;
}