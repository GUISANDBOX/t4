#include "comandosv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

Grafo processaVia(FILE *arqvia_file) {
  char comando[100];
  int z;
  int numvertices;
  char id[100], i[100], j[100], ldir[100], lesq[100], nome[100];
  double x, y, cmp, vm;

  fscanf(arqvia_file, "%d", &numvertices);
  Grafo grafo = criaGrafo(numvertices);
  do {
    z = fscanf(arqvia_file, " %99s", comando);
    if (z != 1)
      break;

    printf("Lendo comando V: %s\n", comando);
    if (strcmp(comando, "v") == 0) {
      fscanf(arqvia_file, "%s %lf %lf", id, &x, &y);
      adicionaVertice(grafo, id, x, y);
    }
    printf("Lendo comando V: %s\n", comando);
    if (strcmp(comando, "e") == 0) {
      fscanf(arqvia_file, "%s %s %s %s %lf %lf %s", i, j, ldir, lesq, &cmp, &vm,
             nome);
      adicionaAresta(grafo, i, j, ldir, lesq, cmp, vm, nome);
    }
  } while (1);
  imprimeGrafo(grafo);
  return grafo;
}
