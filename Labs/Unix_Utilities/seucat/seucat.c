#include "stdio.h"
#include "stdlib.h"

int main(int argc, char **argv)
{
    if (argc == 1)
        return 0;

    for (int i = 1; i < argc; i++)
    {
        FILE *file = fopen(argv[i], "r");
        if (file == NULL)
        {
            fprintf(stdout, "cannot open file\n");
            exit(1);
        }

        int len = 0;
        fseek(file, 0, SEEK_END);
        len = ftell(file);
        char *buffer = (char *)malloc(sizeof(char) * (len + 1));
        if(!buffer)
        {
            fprintf(stderr, "out of memory.\n");
        }

        rewind(file);
        fread(buffer, sizeof(char), len, file);
        

        printf("%s", buffer);
        fclose(file);
        free(buffer);
    }

    return 0;
}
