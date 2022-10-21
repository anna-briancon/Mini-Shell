#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    /*
    char tampon[] = "Hello, world 123\n";
    write(STDOUT_FILENO, tampon, 17);
    printf("size of tampon = %ld\n", sizeof(tampon));
    int nbr = read(STDIN_FILENO, tampon, 17);
    tampon[nbr] = '\0';
    printf("result : %s.\n", tampon);
    system("/bin/ls -al");
    pid_t p = fork();
    if (p == 0)
    {
        printf("[%d] je suis le processus fils\n", getpid());
        printf("[%d] le processus père a le numéro %d\n", getpid(), getppid());
        sleep(1); // wait for 1 second ...
        exit(EXIT_SUCCESS);
    }
    sleep(1); // wait for for 1 second ..
    printf("[%d] je suis le processus père\n", getpid());
    printf("[%d] le processus fils a le numéro %d\n", getpid(), p);
    exit(EXIT_SUCCESS);
    */

    printf("* début simulation\n");
    pid_t p = fork();
    if (p == 0)
    {
        execl("./simuler", "simuler", "Dale Cooper",
               "Are you Laura Palmer ? ", "5", "2", NULL);
        // simuler_personnage("Dale Cooper", "Are you Laura Palmer ? ", 5, 2);
        exit(EXIT_SUCCESS);
    }
    char *arg[] = {"simuler", "Gordon Cole", "WHAT?!", "10", "1", NULL};
    execv("./simuler", arg);
    // simuler_personnage("Gordon Cole", "WHAT?!", 10, 1);
    printf("* fin simulation\n");
    // execl("./hello", "hello", NULL); // recursive call to main application
    exit(EXIT_SUCCESS);
}
