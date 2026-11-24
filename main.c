#include <stdio.h>      // sprintf, printf
#include <stdlib.h>     // atoi, exit
#include <string.h>     // strcmp, strlen
#include <unistd.h>     // fork, execlp, pipe, dup2, read, write, close
#include <sys/wait.h>   // waitpid
#include <pthread.h>    // pthread_create, pthread_join
#include <time.h>       // clock_gettime

int main (int argc, char *argv[]) {

    if (argc != 4) {
        printf("Nombre d'arguments incorrecte.");
        exit(1);
    }
    if (argv[1][0] != 'f' && argv[1][0] != 't') {
        printf("Paramet'es incorrectes.");
        exit(1);
    }
    if (atoi(argv[2]) <= 0) {
        printf("El primer numero ha de ser major que 0.");
        exit(1);
    }
    if (atoi(argv[3]) <= 0) {
        printf("El segon numero ha de ser major que 0.");
        exit(1);
    }

    int n = atoi(argv[2]);

    if (argv[1][0] == 'f') {

        int primers = 0;
        int nproc = 0;

        for (int i = 0; i < n; ++i) {
            int id = fork();
            if (id == 0) {
                
            }
        }

    }

}