#include "comandosqry.h"
#include "criasvg.h"
#include "hashfile.h"
#include "quadra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registrador.h"
#include "grafo.h"

void processaQry(FILE *arqqry, HashFile Hgeo, Grafo grafo, FILE *arqtxt, FILE *arqsvg) {
  char comando[100];
  int z;
  char reg[10], cep[100], reg1[10], reg2[10], cc[100], cr[100];
  char face;
  int num;
  double v, x, y, w, h, vl;
  Registradores regs = criaRegistradores(11);

  fprintf(arqsvg, "<svg width=\"15000\" height=\"10000\" "
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
    if (strcmp(comando, "@o?") == 0) { // Armazena um registrador

      fscanf(arqqry, "%s %s %c %d", reg, cep, &face, &num);
      double px, py;
      Quadra q = (Quadra)buscarHashItem(Hgeo, cep);
      calculaPontoEndereco(q, face, num, &px, &py);
      crialinhapontilhada(px, py, arqsvg, reg);
      int vMaisProximo = verticeMaisProximo(grafo, px, py);
      printf("Vertice mais próximo: %d => %s\n", vMaisProximo, getIdVertice(grafo, vMaisProximo));
      adicionaRegistrador(regs, reg, cep, face, num, px, py, vMaisProximo);
      fprintf(arqtxt, "Coordenadas do registrador %s: (%.2lf, %.2lf)\n", reg, px, py);
      fprintf(arqtxt, "\n");

    } else if (strcmp(comando, "mvm") == 0) {
      fscanf(arqqry, "%lf %lf %lf %lf %lf", &v, &x, &y, &w, &h);
    } else if (strcmp(comando, "regs") == 0) {
      fscanf(arqqry, "%lf", &vl);
    } else if (strcmp(comando, "exp") == 0) {
      fscanf(arqqry, "%lf", &vl);
    } else if (strcmp(comando, "p?") == 0) {
      fscanf(arqqry, "%s %s %s %s", reg1, reg2, cc, cr);
      int vertice1 = getVerticeRegistrador(regs, reg1);
      int vertice2 = getVerticeRegistrador(regs, reg2);
      ResultadoDijkstra resdistancia = dijkstra(grafo, vertice1, vertice2, MENOR_DISTANCIA);
      ResultadoDijkstra restempo = dijkstra(grafo, vertice1, vertice2, MENOR_TEMPO);
      fprintf(arqtxt, "Caminho entre %s (vertice %s) e %s (vertice %s):\n", reg1, getIdVertice(grafo, vertice1), reg2, getIdVertice(grafo, vertice2));

      if (existeCaminho(resdistancia)) {
        imprimeCaminho(grafo, resdistancia, arqtxt);
        escreveCaminhoSVG(grafo, resdistancia, cc, arqsvg);
      } else {
        fprintf(arqtxt, "Nao existe caminho mais curto entre %s e %s\n", reg1, reg2);
      }
      if (existeCaminho(restempo)) {
        imprimeCaminho(grafo, restempo, arqtxt);
        escreveCaminhoSVG(grafo, restempo, cr, arqsvg);
      } else {
        fprintf(arqtxt, "Nao existe caminho mais rapido entre %s e %s\n", reg1, reg2);
      }
      liberaResultadoDijkstra(resdistancia);
      liberaResultadoDijkstra(restempo);
    }
  } while (1);
  fprintf(arqsvg, "</svg>\n");
  //imprimeGrafo(grafo);
  printRegistradores(regs);
}