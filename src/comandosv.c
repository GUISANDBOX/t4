#include "comandosv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void processaVertices(FILE *arqpm_file) {
  char comando[100];
  int z;

  do {
    z = fscanf(arqpm_file, " %99s", comando);
    if (z != 1)
      break;

    printf("Lendo comando PM: %s\n", comando);
    if (strcmp(comando, "v") == 0) {
    }
  } while (1);
}
