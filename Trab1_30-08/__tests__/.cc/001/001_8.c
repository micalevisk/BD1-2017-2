//:description: ler CSV mal formatado (pela stdin)
//:compile: gcc 003_1.c -D SHOW -o 003_1

#include <stdio.h>
#include <stdbool.h>

#define N_FIELDS  7
#define SEPARATOR ';'
#define DELIMITER '"'

int main (void)
{
  char c;
  unsigned nfield;
  unsigned long nrec;
  bool in, eofield; /* end of field */

  nfield = 0;
  nrec = 0;
  in = false;
  eofield = false;

  while ((c = getchar()) != EOF) {
    #ifdef SHOW
    if (c != '\n') {
      printf("%c", c);
    } else if (!in && eofield) {
      printf("\n");
      eofield = false;
    }
    #endif

    if (c == DELIMITER) {
      in = !in;
    } else {
      if (!in && c == SEPARATOR) {
        nfield++;
      }
      if (nfield == N_FIELDS - 1) {
        nrec++;
        nfield = 0;
        eofield = true;
      }
    }
  }

  #ifdef QTD
  printf("Records = %lu\n", nrec);
  #endif

  return 0;
}
