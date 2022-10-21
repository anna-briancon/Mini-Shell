#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void
simuler_personnage( const char nom[], const char phrase[], int repetitions, int secondes )
{
    printf( "%s arrive.\n", nom );
    sleep( secondes );
    for ( int i = 0; i < repetitions; i++ ) {
        printf( "%s: %s\n", nom, phrase );
        sleep( secondes );
    }
    printf( "%s s'en va.\n", nom );
}

struct Personnage {
    const char *nom;
    const char *phrase;
    int         repetitions;
    int         secondes;
};

struct Personnage personnages[] = { { "Dale Cooper", "Are you Laura Palmer?", 5, 2 },
                                    { "Log Lady", "The owls were flying.", 3, 3 },
                                    { "Gordon Cole", "WHAT?!", 10, 1 } };

int
main()
{
    printf( "* début simulation\n" );

    for ( int i = 0; i < 3; i++ ) {
        pid_t p = fork(); // le pere se duplique
        if ( p == 0 ) {  // processus fils
            simuler_personnage( personnages[i].nom,
                                personnages[i].phrase,
                                personnages[i].repetitions,
                                personnages[i].secondes );
            exit( EXIT_SUCCESS );
        }
        // processus pere : (p = pid du processus fils qu'il vient de creer)
    }
    sleep( 60 );  // wait sur les 3 personnages TODO:
    printf( "* fin simulation\n" );
    return 0;
}