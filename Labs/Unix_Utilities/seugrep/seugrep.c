#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getNextIndex(int *next, char *toMatch, int len)
{
    next[0] = -1;
    int i = 0;
    int j = 1;
    while (j < len)
    {
        if (toMatch[next[i] + 1] == toMatch[j])
        {
            next[j] = next[i] + 1;
            j++;
            i = j - 1;
        }
        else
        {
            if (next[i] + 1 == 0)
            {
                next[j] = -1;
                j++;
                i = j - 1;
            }
            else
                i = next[i];
        }
    }
}

int KMP(char *line, char *toMatch, int *next, long int lenS, long int lenP)
{
    int i = 0, j = 0;
    while (i < lenS && j < lenP)
    {
        if (line[i] != toMatch[j])
        {
            if (j == 0)
                i++;
            else
                j = next[j - 1] + 1;
        }
        else
        {
            i++;
            j++;
        }
    }

    if (j != lenP)
        return 0;
    else
        return 1;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("searchterm [file ...]\n");
        exit(1);
    }

    char *toMatch = argv[1];
    int lenP = strlen(toMatch);
    if (lenP == 0)
        return 0;

    int next[lenP];
    getNextIndex(next, toMatch, lenP);

    if (argc == 2)
    {
        char *line = NULL;
        size_t len = 0;
        ssize_t read = 0;
        while ((read = getline(&line, &len, stdin)) != -1)
        {
            long int lenS = read - 1;
            if (KMP(line, toMatch, next, lenS, lenP))
                printf("%s", line);
        }

        return 0;
    }

    for (int i = 2; i < argc; i++)
    {
        FILE *file = fopen(argv[i], "r");

        if (file == NULL)
        {
            printf("cannot open file\n");
            exit(1);
        }

        char *line = NULL;
        size_t len = 0;
        ssize_t read = 0;
        while ((read = getline(&line, &len, file)) != -1)
        {
            long int lenS = read - 1;
            if (KMP(line, toMatch, next, lenS, lenP))
                printf("%s", line);
        }
    }

    return 0;
}