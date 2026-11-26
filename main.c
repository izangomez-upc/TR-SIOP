#include <stdio.h>      // sprintf, printf
#include <stdlib.h>     // atoi, exit
#include <string.h>     // strcmp, strlen
#include <unistd.h>     // fork, execlp, pipe, dup2, read, write, close
#include <sys/wait.h>   // waitpid
#include <pthread.h>    // pthread_create, pthread_join
#include <time.h>       // clock_gettime

int main (int argc, char *argv[]) {

    if (argc != 4) {
        printf("Nombre d'arguments incorrecte.\n");
        exit(1);
    }
    if (argv[1][0] != 'f' && argv[1][0] != 't') {
        printf("Parametres incorrectes.\n");
        exit(1);
    }
    if (atoi(argv[2]) <= 0) {
        printf("El primer numero ha de ser major que 0.\n");
        exit(1);
    }
    if (atoi(argv[3]) <= 0) {
        printf("El segon numero ha de ser major que 0.\n");
        exit(1);
    }

    int n = atoi(argv[2]);
    int limit = atoi(argv[3]);
    int tros = limit / n;
    int primers = 0;

    if (argv[1][0] == 'f') {

        
        int fd[n][2];
        

        for (int i = 0; i < n; ++i) {
            
            int n1 = i * tros + 1;      
            int n2;
            if (i == n - 1) {           //Esto se hace para cuando la division es inexacta, para llegar siempre hasta el ultimo valor.
                n2 = limit;
            }
            else {                     
                n2 = (i+1) * tros;
            }
            
            if (pipe(fd[i]) < 0) {
                printf("Error al pipe\n");
                exit(1);
            }

            int id = fork();

            if (id < 0) {
                printf("fork\n");
                exit(1);
            }

            if (id == 0) {

                int r = comptar_primers(n1, n2);         // calcula su intervalo
                write(fd[i][1], &r, sizeof(int));        // envía el resultado al padre
                close(fd[i][1]);
                exit(0);         
            }
            close(fd[i][1]); 
        }
        

        for (int i = 0; i < n; ++i) {
            int r;
            read(fd[i][0], &r, sizeof(int));
            primers += r;
            close(fd[i][0]);
        }

        while (wait(NULL) > 0);

    }

}
