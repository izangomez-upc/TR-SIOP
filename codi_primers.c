#include <stdlib.h>
#include <math.h>
#include <stdio.h>

int es_primer(int n) {
    int primer = 1;
    for (int i = 2; i*i <= n && primer; ++i) {
        if (n%i == 0) primer = 0;
    }
    return primer;
}

int main(int argc, char *argv[]) {
  int n = atoi(argv[1]);
  int numPrimers = 0;
  for (int i = 2; i <= n; ++i) {
    if (es_primer(i)) ++numPrimers;
  }
  printf("Hi ha %d nombres primers entre l'1 i el %d\n", numPrimers, n);
}
