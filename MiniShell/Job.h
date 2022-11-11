#include <unistd.h>

#include "StringVector.h"

struct Job
{
    pid_t pid;
    char *command;
};