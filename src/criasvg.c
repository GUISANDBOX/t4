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

void crialinhapontilhada(double x, double y, FILE *arq, char *reg) {
  fprintf(arq,
          "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"red\" "
          "stroke-dasharray=\"5,5\" />\n",
          x, y, x, y - 100);
  fprintf(arq, "<text x=\"%lf\" y=\"%lf\" text-anchor=\"middle\" font-weight=\"bold\" >%s</text>\n", x, y - 100, reg);
}

void crialinha(double x1, double y1, double x2, double y2, FILE *arq, char *cor) {
  fprintf(arq,
          "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"%s\" "
          "stroke-width=\"2\" />\n",
          x1, y1, x2, y2, cor);
}