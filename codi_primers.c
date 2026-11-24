#include <stdio.h>

es_primer(int n) {

}

int main(int argc, char *argv[]) {
  int n = 20;
  int numPrimers = 0;
  for (int i = 2; i < n; ++i) {
    if (es_primer(i)) ++numPrimers;
  }
  printf("Hi ha %d nombres primers entre l'1 i el %d", numPrimers, n);
}
