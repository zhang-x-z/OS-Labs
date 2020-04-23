#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        fprintf(stdout, "seuzip: file1 [file2 ...]\n");
        exit(1);
    }

    char last;
    char curr;
    int num = 0;
    for (int i = 1; i < argc; i++)
    {
        FILE *file = fopen(argv[i], "r");
        last = getc(file);
        num++;
        while ((curr = getc(file)) != EOF)
        {
            if (curr == last)
                num++;
            else
            {
                fwrite(&num, 4, 1, stdout);
                fwrite(&last, sizeof(char), 1, stdout);
                num = 1;
                last = curr;
            }
        }
    }
    fwrite(&num, 4, 1, stdout);
    fwrite(&last, sizeof(char), 1, stdout);
}