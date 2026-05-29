#include "criasvg.h"
#include "quadra.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void desenhaSVGQuadra(Quadra q, FILE *arq) {
  fprintf(arq,
          "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" "
          "stroke-width=\"%s\" fill=\"%s\" stroke=\"%s\" />\n",
          getX(q), getY(q), getW(q), getH(q), getSw(q), getCfill(q),
          getCstrk(q));
  fprintf(arq, "<text x=\"%lf\" y=\"%lf\">%s</text>\n", getX(q), getY(q),
          getCep(q));
}

void desenhaXvermelho(Quadra q, FILE *arq) {
  fprintf(arq, "<text x=\"%lf\" y=\"%lf\" stroke=\"red\">X</text>\n", getX(q),
          getY(q));
}
