#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <assert.h>

void simuler_equipe()
{
    // Initialisation de la random seed
    srand( getpid() );

    for ( int j = 1; j <= 4; j++ ) {
        printf( "Le joueur %d de l'équipe %d est parti\n",
                j, getpid() );
        if ( rand() % 10 == 0 ) {
            printf( "Le joueur %d de l'équipe %d chute !\n", j, getpid() );
            exit( EXIT_FAILURE );
        }
        sleep( 8 + ( rand() % 11 ) );
    }
    exit(EXIT_SUCCESS);
}

int main()
{
    printf("* début course\n");
    pid_t pids[3];
    for (int i = 0; i < 6; i++)
    {
        pids[i] = fork(); // le pere se duplique
        if (pids[i] == 0)
        { // processus fils
            simuler_equipe();
            assert(0); // should never reach this ligne !!!
        }
        // processus pere : (p = pid du processus fils qu'il vient de creer)
    }
    int status;
    for (int i = 0; i < 6; i++)
    {
        pid_t p=wait(&status);
        printf("[%d] status : %d\n", p, WEXITSTATUS(status));
    }
    printf("* fin course\n");
    return 0;
}