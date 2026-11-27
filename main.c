#include <stdio.h>      // sprintf, printf
#include <stdlib.h>     // atoi, exit
#include <string.h>     // strcmp, strlen
#include <unistd.h>     // fork, execlp, pipe, dup2, read, write, close
#include <sys/wait.h>   // waitpid
#include <pthread.h>    // pthread_create, pthread_join
#include <time.h>       // clock_gettime

int es_primer(int n) {
  int primer = 1;
  for (int i = 2; i <= n/i && primer; ++i) {
    if (n%i == 0) primer = 0;
  }
  return primer;
}

int comptar_primers(int n1, int n2) {
    int numPrimers = 0;
    int i;
    if (n1 == 1) i = 2;
    else i = n1; 
    while (i <= n2) {
        if (es_primer(i)) ++numPrimers;
        if (i == n2) break;
        ++i;
    }
    return numPrimers;
}

int main (int argc, char *argv[]) {
    if (argc != 4) {
        printf("Nombre d'arguments incorrecte.\n");
        exit(1);
    }
    if (argv[1][0] != 'F' && argv[1][0] != 'T') {
        printf("Parametres incorrectes.\n");
        exit(1);
    }
    int n = atoi(argv[2]);
    int m = atoi(argv[3]);
    if (n <= 0) {
        printf("El primer numero ha de ser major que 0.\n");
        exit(1);
    }
    if (m <= 0) {
        printf("El segon numero ha de ser major que 0.\n");
        exit(1);
    }
    if (n > m) {
        printf("L'interval ha de ser creixent.");
        exit(1);
    }
    int primers = 0;
    if (argv[1][0] == 'F') {
        int files = ((m-n)/10000)+1;
        int fd[files][2];
        int k = 0;
        for (int i = n; i <= m; i += 10000) {
            int n1 = i;
            int n2;
            if (n1 + 9999 > m) n2 = m;
            else n2 = n1 + 9999;
            if (pipe(fd[k]) < 0) {
                printf("Error a la pipe\n.");
                exit(1);
            }
            int id = fork();
            if (id < 0) {
                printf("fork\n");
                exit(1);
            }
            if (id == 0) {
                int r = comptar_primers(n1, n2);         // calcula el seu interval
                write(fd[k][1], &r, sizeof(int));        // envia el resultat al pare
                close(fd[k][1]);
                exit(0);         
            }
            close(fd[k][1]); 
            ++k;
        }
        k = 0;
        for (int i = n; i <= m; i += 10000) {
            int r;
            read(fd[k][0], &r, sizeof(int));
            primers += r;
            close(fd[k][0]);
            ++k;
        }
        while (wait(NULL) > 0);
    }
    printf("%d\n", primers);
}
