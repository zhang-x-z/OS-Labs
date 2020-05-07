#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct path
{
    char **paths;
    int num;
} path;

void error()
{
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

void runCmd(char *cmd)
{
    char *args[] = {cmd, NULL};
    int pid = fork();
    if (pid == 0)
    {
        fprintf(stdout, "%s run\n", cmd);
        execv(args[0], args);
    }
    else
        wait(0);
}

int main(int argc, char **argv)
{
    if (argc > 2)
    {
        error();
        exit(1);
    }

    if (argc == 1)
    {
        fprintf(stdout, "seush>");
        __ssize_t read = 0;
        size_t len = 0;
        char *cmd;
        while ((read = getline(&cmd, &len, stdin)) != EOF)
        {
            runCmd(cmd);
            fprintf(stdout, "seush>");
        }
        exit(0);
    }
    else
    {
    }
}