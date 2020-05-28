#include <stdio.h>

int main()
{
    printf("program start.\n");
    char *n = NULL;
    //char s = *(n + 1);
    printf("%s\n", n + 1);
    printf("program end.\n");
    return 0;
}