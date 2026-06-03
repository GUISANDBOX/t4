#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>

typedef void *Grafo;

Grafo criaGrafo(int capacidadeVertices);

int adicionaVertice(Grafo g, const char *id, double x, double y);

int adicionaAresta(Grafo g, const char *idOrigem, const char *idDestino,
                   const char *cepDir, const char *cepEsq, double comprimento,
                   double velocidade, const char *nomeRua);

void imprimeGrafo(Grafo g);

void liberaGrafo(Grafo g);

#endif