#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

int es_primer(int n) {
    int primer = 1;
    for (int i = 2; i*i <= n && primer; ++i) {
        if (n%i == 0) primer = 0;
    }
    return primer;
}

int main(int argc, char *argv[]) {
  int n = atoi(argv[1]);
  int m = atoi(argv[2]);
  int numPrimers = 0;
  int i;
  if (n == 1) i = 2;
  else i = n; 
  while (i <= m) {
    if (es_primer(i)) ++numPrimers;
    ++i;
  }
  char buffer[12];
  snprintf(buffer, sizeof(buffer), "%d", numPrimers);
  write(1, buffer, strlen(buffer));
}