#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "criasvg.h"

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

/* Algoritmo de Dijkstra */
struct sResultadoDijkstra {
    double custoTotal;

    int origem;
    int destino;
    int qtdVertices;

    int *anterior;
    Aresta *arestaAnterior;

    CriterioDijkstra criterio;
};

typedef struct sResultadoDijkstra *ResultadoInterno;

static double pesoAresta(Aresta a, CriterioDijkstra criterio) {
    if (a == NULL) {
        return INF;
    }

    if (criterio == MENOR_DISTANCIA) {
        return a->comprimento;
    }

    if (criterio == MENOR_TEMPO) {
        if (a->velocidade <= 0) {
            return INF;
        }

        return a->comprimento / a->velocidade;
    }

    return a->comprimento;
}

ResultadoDijkstra dijkstra(
    Grafo grafo,
    int origem,
    int destino,
    CriterioDijkstra criterio
) {
    if (grafo == NULL) {
        return NULL;
    }

    GrafoInterno g = (GrafoInterno) grafo;

    int n = g->qtdVertices;

    if (origem < 0 || origem >= n || destino < 0 || destino >= n) {
        return NULL;
    }

    ResultadoInterno resultado = malloc(sizeof(struct sResultadoDijkstra));

    if (resultado == NULL) {
        return NULL;
    }

    double *dist = malloc(n * sizeof(double));
    int *visitado = malloc(n * sizeof(int));
    int *anterior = malloc(n * sizeof(int));
    Aresta *arestaAnterior = malloc(n * sizeof(Aresta));

    if (
        dist == NULL ||
        visitado == NULL ||
        anterior == NULL ||
        arestaAnterior == NULL
    ) {
        free(dist);
        free(visitado);
        free(anterior);
        free(arestaAnterior);
        free(resultado);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visitado[i] = 0;
        anterior[i] = -1;
        arestaAnterior[i] = NULL;
    }

    dist[origem] = 0;

    for (int cont = 0; cont < n; cont++) {
        int u = -1;
        double menor = INF;

        for (int i = 0; i < n; i++) {
            if (!visitado[i] && dist[i] < menor) {
                menor = dist[i];
                u = i;
            }
        }

        if (u == -1) {
            break;
        }

        if (dist[u] >= INF / 2) {
            break;
        }

        visitado[u] = 1;

        if (u == destino) {
            break;
        }

        Aresta atual = g->vertices[u].listaAdj;

        while (atual != NULL) {
            int v = atual->destino;

            double peso = pesoAresta(atual, criterio);

            if (!visitado[v] && peso < INF / 2) {
                double novaDist = dist[u] + peso;

                if (novaDist < dist[v]) {
                    dist[v] = novaDist;
                    anterior[v] = u;
                    arestaAnterior[v] = atual;
                }
            }

            atual = atual->prox;
        }
    }

    resultado->custoTotal = dist[destino];
    resultado->origem = origem;
    resultado->destino = destino;
    resultado->qtdVertices = n;
    resultado->anterior = anterior;
    resultado->arestaAnterior = arestaAnterior;
    resultado->criterio = criterio;

    free(dist);
    free(visitado);

    return resultado;
}

int existeCaminho(ResultadoDijkstra resultado) {
    if (resultado == NULL) {
        return 0;
    }

    ResultadoInterno r = (ResultadoInterno) resultado;

    return r->custoTotal < INF / 2;
}

void imprimeCaminho(
    Grafo grafo,
    ResultadoDijkstra resultado,
    FILE *arqtxt
) {
    if (grafo == NULL || resultado == NULL) {
        return;
    }

    GrafoInterno g = (GrafoInterno) grafo;
    ResultadoInterno r = (ResultadoInterno) resultado;

    if (!existeCaminho(resultado)) {
        printf("Nao existe caminho entre %s e %s\n",

               g->vertices[r->origem].id,
               g->vertices[r->destino].id);
        if (arqtxt != NULL) {
            fprintf(arqtxt, "Nao existe caminho entre %s e %s\n",
                    
                   g->vertices[r->origem].id,
                   g->vertices[r->destino].id);
        }
        return;
    }

    int *pilha = malloc(r->qtdVertices * sizeof(int));

    if (pilha == NULL) {
        return;
    }

    int topo = 0;
    int atual = r->destino;

    while (atual != -1) {
        pilha[topo] = atual;
        topo++;

        if (atual == r->origem) {
            break;
        }

        atual = r->anterior[atual];
    }

    printf("Caminho por vertices:\n");
    if (arqtxt != NULL) {
        fprintf(arqtxt, "Caminho por vertices:\n");
    }

    for (int i = topo - 1; i >= 0; i--) {
        int v = pilha[i];

        printf("%s", g->vertices[v].id);
        if (arqtxt != NULL) fprintf(arqtxt, "%s", g->vertices[v].id);

        if (i > 0) {
            printf(" -> ");
            if (arqtxt != NULL) fprintf(arqtxt, " -> ");
        }
    }

    printf("\n\n");
    if (arqtxt != NULL) fprintf(arqtxt, "\n\n");

    printf("Detalhes do caminho:\n");
    if (arqtxt != NULL) fprintf(arqtxt, "Detalhes do caminho:\n");

    for (int i = topo - 1; i > 0; i--) {
        int u = pilha[i];
        int v = pilha[i - 1];

        Aresta a = r->arestaAnterior[v];

        if (a != NULL) {
            printf(
                "%s -> %s | rua: %s | comp: %.2lf | vel: %.2lf",
                g->vertices[u].id,
                g->vertices[v].id,
                a->nomeRua,
                a->comprimento,
                a->velocidade
            );
            if (arqtxt != NULL) fprintf(arqtxt, "%s -> %s | rua: %s | comp: %.2lf | vel: %.2lf",
                    g->vertices[u].id,
                    g->vertices[v].id,
                    a->nomeRua,
                    a->comprimento,
                    a->velocidade
                );

            if (r->criterio == MENOR_DISTANCIA) {
                printf(" | peso usado: %.2lf", a->comprimento);
                if (arqtxt != NULL) fprintf(arqtxt, " | peso usado: %.2lf", a->comprimento);
            } else {
                printf(" | tempo usado: %.4lf", pesoAresta(a, MENOR_TEMPO));
                if (arqtxt != NULL) fprintf(arqtxt, " | tempo usado: %.4lf", pesoAresta(a, MENOR_TEMPO));
            }

            printf("\n");
            if (arqtxt != NULL) fprintf(arqtxt, "\n");
        }
    }

    if (r->criterio == MENOR_DISTANCIA) {
        printf("\nCusto total em distancia: %.2lf\n", r->custoTotal);
        if (arqtxt != NULL) fprintf(arqtxt, "\nCusto total em distancia: %.2lf\n", r->custoTotal);
    } else {
        printf("\nCusto total em tempo: %.4lf\n", r->custoTotal);
        if (arqtxt != NULL) fprintf(arqtxt, "\nCusto total em tempo: %.4lf\n", r->custoTotal);
    }

    free(pilha);
}

double getCustoResultado(ResultadoDijkstra resultado) {
    if (resultado == NULL) {
        return INF;
    }

    ResultadoInterno r = (ResultadoInterno) resultado;

    return r->custoTotal;
}

void liberaResultadoDijkstra(ResultadoDijkstra resultado) {
    if (resultado == NULL) {
        return;
    }

    ResultadoInterno r = (ResultadoInterno) resultado;

    free(r->anterior);
    free(r->arestaAnterior);
    free(r);
}

void escreveCaminhoSVG(Grafo grafo, ResultadoDijkstra resultado, char* cor, FILE *arqsvg){
    if (grafo == NULL || resultado == NULL) {
        return;
    }

    GrafoInterno g = (GrafoInterno) grafo;
    ResultadoInterno r = (ResultadoInterno) resultado;

    if (!existeCaminho(resultado)) {
        printf("Nao existe caminho entre %s e %s\n",
               g->vertices[r->origem].id,
               g->vertices[r->destino].id);
        return;
    }

    int *pilha = malloc(r->qtdVertices * sizeof(int));

    if (pilha == NULL) {
        return;
    }

    int topo = 0;
    int atual = r->destino;

    while (atual != -1) {
        pilha[topo] = atual;
        topo++;

        if (atual == r->origem) {
            break;
        }

        atual = r->anterior[atual];
    }

    for (int i = topo - 1; i > 0; i--) {
        int u = pilha[i];
        int v = pilha[i - 1];

        if(i==topo-1){
            fprintf(arqsvg, "<text x=\"%lf\" y=\"%lf\" text-anchor=\"middle\" font-weight=\"bold\" >%s</text>\n", g->vertices[u].x, g->vertices[u].y - 10, "I");
        }

        crialinha(g->vertices[u].x, g->vertices[u].y, g->vertices[v].x, g->vertices[v].y, arqsvg, cor);
    }

    fprintf(arqsvg, "<text x=\"%lf\" y=\"%lf\" text-anchor=\"middle\" font-weight=\"bold\" >%s</text>\n", g->vertices[r->destino].x, g->vertices[r->destino].y - 10, "F");

    free(pilha);
}