#include "comandosqry.h"
#include "criasvg.h"
#include "hashfile.h"
#include "quadra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void processaQry(FILE *arqqry, HashFile Hgeo, FILE *arqtxt, FILE *arqsvg) {
  char comando[100];
  int z;

  fprintf(arqsvg, "<svg width=\"5000\" height=\"5000\" "
                  "xmlns=\"http://www.w3.org/2000/svg\">\n");

  HashItem *listaQuadras = malloc(100000 * sizeof(HashItem));
  int numQuadras = getListaItens(Hgeo, listaQuadras);
  for (int i = 0; i < numQuadras; i++) {
    Quadra q = (Quadra)listaQuadras[i];
    desenhaSVGQuadra(q, arqsvg);
    free(q);
  }
  free(listaQuadras);
  do {
    z = fscanf(arqqry, " %99s", comando);
    if (z != 1)
      break;

    printf("Lendo comando QRY: %s\n", comando);
    if (strcmp(comando, "rq") == 0) {

    } else if (strcmp(comando, "pq") == 0) {

    } else if (strcmp(comando, "censo") == 0) {

    } else if (strcmp(comando, "h?") == 0) {
    }
  } while (1);
  fprintf(arqsvg, "</svg>\n");
}