#ifndef GRAFO_H
#define GRAFO_H
#define INF 1e100
#include <stdio.h>

typedef void *Grafo;
typedef void *ResultadoDijkstra;

typedef enum {
    MENOR_DISTANCIA,
    MENOR_TEMPO
} CriterioDijkstra;

Grafo criaGrafo(int capacidadeVertices);

int adicionaVertice(Grafo g, const char *id, double x, double y);

int adicionaAresta(Grafo g, const char *idOrigem, const char *idDestino,
                   const char *cepDir, const char *cepEsq, double comprimento,
                   double velocidade, const char *nomeRua);

void imprimeGrafo(Grafo g);

void liberaGrafo(Grafo g);

int buscaIndiceVertice(Grafo g, const char *id);

int verticeMaisProximo(
    Grafo g,
    double x,
    double y
);

int getQuantidadeVertices(Grafo g);

int getQuantidadeArestas(Grafo g);

char *getIdVertice(Grafo g, int indice);
double getXVertice(Grafo g, int indice);
double getYVertice(Grafo g, int indice);


ResultadoDijkstra dijkstra(
    Grafo g,
    int origem,
    int destino,
    CriterioDijkstra criterio
);

void imprimeCaminho(
    Grafo g,
    ResultadoDijkstra resultado,
    FILE *arqtxt
);

double getCustoResultado(
    ResultadoDijkstra resultado
);

int existeCaminho(
    ResultadoDijkstra resultado
);

void liberaResultadoDijkstra(
    ResultadoDijkstra resultado
);

int getQuantidadeVertices(Grafo g);

void escreveCaminhoSVG(Grafo g, ResultadoDijkstra resultado, char* cor, FILE *arqsvg);


#endif