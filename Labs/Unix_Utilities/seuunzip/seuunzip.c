#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        fprintf(stdout, "seuunzip: file1 [file2 ...]\n");
        exit(1);
    }

    for (int i = 1; i < argc; i++)
    {
        FILE *file = fopen(argv[i], "r");
        int num;
        char c;
        size_t size;
        while ((size = fread(&num, 4, 1, file)) == 1)
        {
            fread(&c, sizeof(char), 1, file);
            for (int j = 0; j < num; j++)
            {
                printf("%c", c);
            }
        }
    }
}