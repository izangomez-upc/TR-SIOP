#include <stdio.h>      // printf
#include <stdlib.h>     // atoi, exit
#include <unistd.h>     // fork, pipe, read, write, close
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

/* Codigo que ejecuta cada thread (equivale al bloque "if (pid == 0)" del fork) */
void *fil(void *arg) {
    int *dades = arg;                                // dades[0] = n1, dades[1] = n2, dades[2] = resultat
    int n1 = dades[0];
    int n2 = dades[1];
    int resultat = comptar_primers(n1, n2);          // calcula su intervalo
    dades[2] = resultat;                             // deja el resultado para el main
    return NULL;                                     // termina el thread (equivale al exit(0))
}

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

        int fd[n][2];                                    // un pipe por hijo: [0] = lectura, [1] = escritura

        for (int i = 0; i < n; ++i) {

            int n1 = i * tros + 1;
            int n2;
            if (i == n - 1) {           // Para cuando la division es inexacta, para llegar siempre hasta el ultimo valor.
                n2 = limit;
            }
            else {
                n2 = (i + 1) * tros;
            }

            if (pipe(fd[i]) < 0) {
                printf("Error al pipe\n");
                exit(1);
            }

            int pid = fork();

            if (pid < 0) {
                printf("Error al fork\n");
                exit(1);
            }

            if (pid == 0) {
                int resultat = comptar_primers(n1, n2);          // calcula su intervalo
                write(fd[i][1], &resultat, sizeof(int));         // envía el resultado al padre
                close(fd[i][1]);
                exit(0);
            }

            close(fd[i][1]);                                     // el padre no escribe
        }

        for (int i = 0; i < n; ++i) {
            int resultat;
            read(fd[i][0], &resultat, sizeof(int));              // recoger el resultado del hijo i
            primers += resultat;
            close(fd[i][0]);
        }

        while (waitpid(-1, NULL, 0) > 0);                        // esperar a todos los hijos
    }
    else if (argv[1][0] == 't') {

        int dades[n][3];                                         // por thread: [0] = n1, [1] = n2, [2] = resultat
        pthread_t thread_id[n];                                  // identificador de cada thread

        for (int i = 0; i < n; ++i) {

            int n1 = i * tros + 1;
            int n2;
            if (i == n - 1) {           // Para cuando la division es inexacta, para llegar siempre hasta el ultimo valor.
                n2 = limit;
            }
            else {
                n2 = (i + 1) * tros;
            }

            dades[i][0] = n1;                                    // entrada del thread
            dades[i][1] = n2;

            if (pthread_create(&thread_id[i], NULL, fil, dades[i]) != 0) {
                printf("Error al pthread_create\n");
                exit(1);
            }
        }

        for (int i = 0; i < n; ++i) {
            pthread_join(thread_id[i], NULL);                    // esperar al thread i
            int resultat = dades[i][2];                          // recoger su resultado
            primers += resultat;
        }
    }

    printf("%d\n", primers);
    return 0;
}
