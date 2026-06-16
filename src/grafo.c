#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_ID 64
#define TAM_CEP 64
#define TAM_RUA 128


typedef struct sAresta *Aresta;

struct sAresta {
  int destino;

  char cepDir[TAM_CEP];
  char cepEsq[TAM_CEP];

  double comprimento;
  double velocidade;

  char nomeRua[TAM_RUA];

  Aresta prox;
};

struct sVertice {
  int indice;

  char id[TAM_ID];

  double x;
  double y;

  Aresta listaAdj;
};

struct sGrafo {
  struct sVertice *vertices;

  int capacidadeVertices;
  int qtdVertices;
  int qtdArestas;
};

typedef struct sGrafo *GrafoInterno;


static void copiaString(char *destino, const char *origem, int tamanho) {
  if (origem == NULL) {
    destino[0] = '\0';
    return;
  }

  strncpy(destino, origem, tamanho - 1);
  destino[tamanho - 1] = '\0';
}

Grafo criaGrafo(int capacidadeVertices) {
  GrafoInterno g = malloc(sizeof(struct sGrafo));

  if (g == NULL) {
    printf("Erro ao alocar grafo\n");
    return NULL;
  }

  g->vertices = malloc(capacidadeVertices * sizeof(struct sVertice));

  if (g->vertices == NULL) {
    printf("Erro ao alocar vetor de vertices\n");
    free(g);
    return NULL;
  }

  g->capacidadeVertices = capacidadeVertices;
  g->qtdVertices = 0;
  g->qtdArestas = 0;

  for (int i = 0; i < capacidadeVertices; i++) {
    g->vertices[i].indice = i;
    g->vertices[i].id[0] = '\0';
    g->vertices[i].x = 0;
    g->vertices[i].y = 0;
    g->vertices[i].listaAdj = NULL;
  }

  return g;
}

static int buscaIndice(GrafoInterno g, const char *id) {
    if (g == NULL || id == NULL) {
        return -1;
    }

    for (int i = 0; i < g->qtdVertices; i++) {
        if (strcmp(g->vertices[i].id, id) == 0) {
            return i;
        }
    }

    return -1;
}

int adicionaVertice(
    Grafo grafo,
    const char *id,
    double x,
    double y
) {
    if (grafo == NULL || id == NULL) {
        return 0;
    }

    GrafoInterno g = (GrafoInterno) grafo;

    if (g->qtdVertices >= g->capacidadeVertices) {
        printf("Capacidade maxima de vertices atingida\n");
        return 0;
    }

    if (buscaIndice(g, id) != -1) {
        printf("Vertice com ID repetido: %s\n", id);
        return 0;
    }

    int indice = g->qtdVertices;

    g->vertices[indice].indice = indice;
    copiaString(g->vertices[indice].id, id, TAM_ID);
    g->vertices[indice].x = x;
    g->vertices[indice].y = y;
    g->vertices[indice].listaAdj = NULL;

    g->qtdVertices++;

    return 1;
}

int adicionaAresta(
    Grafo grafo,
    const char *idOrigem,
    const char *idDestino,
    const char *cepDir,
    const char *cepEsq,
    double comprimento,
    double velocidade,
    const char *nomeRua
) {
    if (grafo == NULL || idOrigem == NULL || idDestino == NULL) {
        return 0;
    }

    GrafoInterno g = (GrafoInterno) grafo;

    int origem = buscaIndice(g, idOrigem);
    int destino = buscaIndice(g, idDestino);

    if (origem == -1) {
        printf("Vertice de origem nao encontrado: %s\n", idOrigem);
        return 0;
    }

    if (destino == -1) {
        printf("Vertice de destino nao encontrado: %s\n", idDestino);
        return 0;
    }

    Aresta nova = malloc(sizeof(struct sAresta));

    if (nova == NULL) {
        printf("Erro ao alocar aresta\n");
        return 0;
    }

    nova->destino = destino;

    copiaString(nova->cepDir, cepDir, TAM_CEP);
    copiaString(nova->cepEsq, cepEsq, TAM_CEP);
    copiaString(nova->nomeRua, nomeRua, TAM_RUA);

    nova->comprimento = comprimento;
    nova->velocidade = velocidade;

    nova->prox = g->vertices[origem].listaAdj;
    g->vertices[origem].listaAdj = nova;

    g->qtdArestas++;

    return 1;
}

void imprimeGrafo(Grafo grafo) {
    if (grafo == NULL) {
        return;
    }

    GrafoInterno g = (GrafoInterno) grafo;

    printf("Quantidade de vertices: %d\n", g->qtdVertices);
    printf("Quantidade de arestas: %d\n\n", g->qtdArestas);

    for (int i = 0; i < g->qtdVertices; i++) {
        printf(
            "[%d] %s (%.2lf, %.2lf)\n",
            g->vertices[i].indice,
            g->vertices[i].id,
            g->vertices[i].x,
            g->vertices[i].y
        );

        Aresta atual = g->vertices[i].listaAdj;

        while (atual != NULL) {
            int d = atual->destino;

            printf(
                "  -> [%d] %s | cepDir: %s | cepEsq: %s | comp: %.2lf | vel: %.2lf | rua: %s\n",
                d,
                g->vertices[d].id,
                atual->cepDir,
                atual->cepEsq,
                atual->comprimento,
                atual->velocidade,
                atual->nomeRua
            );

            atual = atual->prox;
        }

        printf("\n");
    }
}

void liberaGrafo(Grafo grafo) {
    if (grafo == NULL) {
        return;
    }

    GrafoInterno g = (GrafoInterno) grafo;

    for (int i = 0; i < g->qtdVertices; i++) {
        Aresta atual = g->vertices[i].listaAdj;

        while (atual != NULL) {
            Aresta prox = atual->prox;
            free(atual);
            atual = prox;
        }
    }

    free(g->vertices);
    free(g);
}

int buscaIndiceVertice(Grafo grafo, const char *id) {
    if (grafo == NULL || id == NULL) {
        return -1;
    }

    struct sGrafo *g = (struct sGrafo *) grafo;

    for (int i = 0; i < g->qtdVertices; i++) {
        if (strcmp(g->vertices[i].id, id) == 0) {
            return i;
        }
    }

    return -1;
}

int verticeMaisProximo(Grafo grafo, double x, double y) {
    if (grafo == NULL) {
        return -1;
    }

    struct sGrafo *g = (struct sGrafo *) grafo;

    int melhor = -1;
    double menorDist = 0;

    for (int i = 0; i < g->qtdVertices; i++) {
        double dx = x - g->vertices[i].x;
        double dy = y - g->vertices[i].y;
        double dist = dx * dx + dy * dy;

        if (melhor == -1 || dist < menorDist) {
            melhor = i;
            menorDist = dist;
        }
    }

    return melhor;
}

int getQuantidadeVertices(Grafo g) {
    if (g == NULL) {
        return 0;
    }

    struct sGrafo *grafo = (struct sGrafo *) g;
    return grafo->qtdVertices;
}

char *getIdVertice(Grafo g, int indice) {
    if (g == NULL || indice < 0) {
        return NULL;
    }

    struct sGrafo *grafo = (struct sGrafo *) g;

    if (indice >= grafo->qtdVertices) {
        return NULL;
    }

    return grafo->vertices[indice].id;
}

double getXVertice(Grafo g, int indice) {
    if (g == NULL || indice < 0) {
        return 0;
    }

    struct sGrafo *grafo = (struct sGrafo *) g;

    if (indice >= grafo->qtdVertices) {
        return 0;
    }

    return grafo->vertices[indice].x;
}

double getYVertice(Grafo g, int indice) {
    if (g == NULL || indice < 0) {
        return 0;
    }

    struct sGrafo *grafo = (struct sGrafo *) g;

    if (indice >= grafo->qtdVertices) {
        return 0;
    }

    return grafo->vertices[indice].y;
}

int getQuantidadeArestas(Grafo g) {
    if (g == NULL) {
        return 0;
    }

    struct sGrafo *grafo = (struct sGrafo *) g;
    return grafo->qtdArestas;
}