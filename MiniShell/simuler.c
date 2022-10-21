#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void simuler_personnage(const char nom[], const char phrase[],
                        int repetitions, int secondes)
{
    printf("%s arrive.\n", nom);
    sleep(secondes);
    for (int i = 0; i < repetitions; i++)
    {
        printf("%s: %s\n", nom, phrase);
        sleep(secondes);
    }
    printf("%s s'en va.\n", nom);
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("usage : simuler nom phrase repetitions seconds\n");
        exit(EXIT_FAILURE);
    }
    int repetitions = atoi(argv[3]);
    int seconds = atoi(argv[4]);

    simuler_personnage(argv[1], argv[2], repetitions, seconds);

    exit(EXIT_SUCCESS);
}