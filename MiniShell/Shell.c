#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Shell.h"
#include "StringVector.h"

void shell_init(struct Shell *this)
{
    this->running = false;
    this->line = NULL;
    this->line_number = 0;
    this->line_length = 0;
}

void shell_free(struct Shell *this)
{
    if (NULL != this->line)
    {
        free(this->line);
        this->line = NULL;
    }
    this->line_length = 0;
}

void shell_run(struct Shell *this)
{
    this->running = true;
    printf("* Shell started\n");
    while (this->running)
    {
        shell_read_line(this);
        shell_execute_line(this);
    }
    printf("* Shell stopped\n");
}

void shell_read_line(struct Shell *this)
{
    this->line_number++;
    char *buf = getcwd(NULL, 0);
    printf("%d: %s> ", this->line_number, buf);
    free(buf);
    getline(&this->line, &this->line_length, stdin);
}

static void
do_help(struct Shell *this, const struct StringVector *args)
{
    printf("-> commands: exit, cd, help, ?.\n");
    (void)this;
    (void)args;
}

static void
do_outpout(char *fileOut)
{
    int fd = open(fileOut,
                  O_WRONLY | O_CREAT, 0777);
    if (fd == -1)
    {
        write(STDOUT_FILENO, "Error opening file", 19);
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

static void
do_input(char *fileIn)
{
    int fd = open(fileIn,
                  O_RDONLY, 0777);
    if (fd == -1)
    {
        write(STDOUT_FILENO, "Error opening file", 19);
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

static void
do_system(struct Shell *this, const struct StringVector *args)
{
    char *file = string_vector_get(args, 1);
    int stringSize = (int)string_vector_size(args);
    char *arguments[stringSize - 1];
    int i = 0;
    char *temp[stringSize - 1];
    int s;
    int end = 0;

    if (fork() == 0)
    {
        while (i < stringSize - 1)
        {
            temp[i] = args->strings[i + 1];

            if (strcmp(temp[i], ">") == 0)
            {
                do_outpout(string_vector_get(args, i + 2));
                i++;
            }
            else if (strcmp(temp[i], "<") == 0)
            {
                do_input(string_vector_get(args, i + 2));
                i++;
            }
            else
            {
                end = i + 1;
                arguments[i] = temp[i];
            }
            i++;
        }
        arguments[end] = NULL;
        execvp(file, arguments);
    }
    wait(&s);
    // faire un free
    (void)this;
}

static void
do_cd(struct Shell *this, const struct StringVector *args)
{
    int nb_tokens = string_vector_size(args);
    char *tmp;
    if (1 == nb_tokens)
    {
        tmp = getenv("HOME");
        printf("Cas 1: %s\n", tmp);
    }
    else
    {
        tmp = string_vector_get(args, 1);
        printf("Cas 2: %s\n", tmp);
    }
    int rc = chdir(tmp);
    if (0 != rc)
        printf("directory '%s' not valid\n", tmp);
    (void)this;
}

static void
do_pwd(struct Shell *this, const struct StringVector *args)
{
    char *buf = getcwd(NULL, 0);
    printf("%s\n", buf);
    (void)this;
    (void)args;
}

static void
do_rappel(struct Shell *this, const struct StringVector *args)
{
    (void)this;
    (void)args;
}

static void
do_execute(struct Shell *this, const struct StringVector *args)
{
    (void)this;
    (void)args;
}

static void
do_exit(struct Shell *this, const struct StringVector *args)
{
    this->running = false;
    (void)this;
    (void)args;
}

typedef void (*Action)(struct Shell *, const struct StringVector *);

static struct
{
    const char *name;
    Action action;
} actions[] = {{.name = "exit", .action = do_exit},
               {.name = "cd", .action = do_cd},
               {.name = "rappel", .action = do_rappel},
               {.name = "help", .action = do_help},
               {.name = "?", .action = do_help},
               {.name = "!", .action = do_system},
               {.name = "pwd", .action = do_pwd},
               {.name = NULL, .action = do_execute}};

Action
get_action(char *name)
{
    int i = 0;
    while (actions[i].name != NULL && strcmp(actions[i].name, name) != 0)
    {
        i++;
    }
    return actions[i].action;
}

void shell_execute_line(struct Shell *this)
{
    struct StringVector tokens = split_line(this->line);
    int nb_tokens = string_vector_size(&tokens);

    if (nb_tokens == 0)
    {
        printf("-> Nothing to do !\n");
    }
    else
    {
        char *name = string_vector_get(&tokens, 0);
        Action action = get_action(name);
        action(this, &tokens);
    }

    string_vector_free(&tokens);
}
