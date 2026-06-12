#include "comandosqry.h"
#include "criasvg.h"
#include "hashfile.h"
#include "quadra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void processaQry(FILE *arqqry, HashFile Hgeo, Grafo grafo, FILE *arqtxt, FILE *arqsvg) {
  char comando[100];
  int z;
  char reg[10], cep[100], reg1[10], reg2[10], cc[100], cr[100];
  char face;
  int num;
  double v, x, y, w, h, vl;

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
    if (strcmp(comando, "@o?") == 0) {
      fscanf(arqqry, "%s %s %c %d", reg, cep, &face, &num);
    } else if (strcmp(comando, "mvm") == 0) {
      fscanf(arqqry, "%lf %lf %lf %lf %lf", &v, &x, &y, &w, &h);
    } else if (strcmp(comando, "regs") == 0) {
      fscanf(arqqry, "%lf", &vl);
    } else if (strcmp(comando, "exp") == 0) {
      fscanf(arqqry, "%lf", &vl);
    } else if (strcmp(comando, "p?") == 0) {
      fscanf(arqqry, "%s %s %s %s", reg1, reg2, cc, cr);
    }
  } while (1);
  fprintf(arqsvg, "</svg>\n");
}