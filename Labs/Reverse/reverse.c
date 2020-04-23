#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct
{
    char *line;
    struct linkedStack *next;
} linkedStack;

int main(int argc, char **argv)
{
    if (argc > 3)
    {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    if (argc == 1)
    {
        __ssize_t read;
        size_t len = 0;
        linkedStack head;
        getline(&head.line, &len, stdin);
        head.next = NULL;
        linkedStack *curr = &head;
        linkedStack *new = malloc(sizeof(linkedStack));
        if (new == NULL)
        {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        while ((read = getline(&new->line, &len, stdin)) != -1)
        {
            new->next = curr;
            curr = new;
            new = malloc(sizeof(linkedStack));
            if (new == NULL)
            {
                fprintf(stderr, "malloc failed\n");
                exit(1);
            }
        }
        while (curr != NULL)
        {
            fprintf(stdout, curr->line);
            curr = curr->next;
        }
        exit(0);
    }

    if (argc == 2)
    {
        FILE *file = fopen(argv[1], "r");
        if (file == NULL)
        {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }

        __ssize_t read;
        size_t len = 0;
        linkedStack head;
        getline(&head.line, &len, file);
        head.next = NULL;
        linkedStack *curr = &head;
        linkedStack *new = malloc(sizeof(linkedStack));
        if (new == NULL)
        {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        while ((read = getline(&new->line, &len, file)) != -1)
        {
            new->next = curr;
            curr = new;
            new = malloc(sizeof(linkedStack));
            if (new == NULL)
            {
                fprintf(stderr, "malloc failed\n");
                exit(1);
            }
        }
        while (curr != NULL)
        {
            fprintf(stdout, curr->line);
            curr = curr->next;
        }
        exit(0);
    }

    if (argc == 3)
    {
        struct stat out_s;
        struct stat in_s;
        stat(argv[2], &out_s);
        stat(argv[1], &in_s);
        if (out_s.st_ino == in_s.st_ino)
        {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(1);
        }

        FILE *in = fopen(argv[1], "r");
        if (in == NULL)
        {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
        FILE *out = fopen(argv[2], "w");

        __ssize_t read;
        size_t len = 0;
        linkedStack head;
        getline(&head.line, &len, in);
        head.next = NULL;
        linkedStack *curr = &head;
        linkedStack *new = malloc(sizeof(linkedStack));
        if (new == NULL)
        {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        while ((read = getline(&new->line, &len, in)) != -1)
        {
            new->next = curr;
            curr = new;
            new = malloc(sizeof(linkedStack));
            if (new == NULL)
            {
                fprintf(stderr, "malloc failed\n");
                exit(1);
            }
        }
        while (curr != NULL)
        {
            fprintf(out, curr->line);
            curr = curr->next;
        }
        exit(0);
    }
}