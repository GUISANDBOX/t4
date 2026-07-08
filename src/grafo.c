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
    double tempoTotal;

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

    double *dist = malloc(n * sizeof(double)); // Array de custo
    double *tempo = malloc(n * sizeof(double)); // Array de tempo
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
    tempo[origem] = 0;
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
            double tempoAresta = pesoAresta(atual, MENOR_TEMPO);

            if (!visitado[v] && peso < INF) {
                double novaDist = dist[u] + peso;
                double novoTempo = tempo[u] + tempoAresta;

                if (novaDist < dist[v]) {
                    dist[v] = novaDist;
                    tempo[v] = novoTempo;
                    anterior[v] = u;
                    arestaAnterior[v] = atual;
                    
                }
            }

            atual = atual->prox;
        }
    }
    
    resultado->custoTotal = dist[destino];
    resultado->tempoTotal = tempo[destino]/10;
    printf("Custo total do caminho: %.2lf\n", resultado->custoTotal);
    printf("Tempo total do caminho: %.4lf\n", resultado->tempoTotal);
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

    fprintf(arqsvg,  "<text x=\"%lf\" y=\"%lf\" text-anchor=\"middle\" font-weight=\"bold\" >%s</text>\n", g->vertices[r->origem].x, g->vertices[r->origem].y - 10, "I");

    fprintf(arqsvg, "<path d=\"M ");

    for (int i = topo - 1; i > 0; i--) {
        int u = pilha[i];
        int v = pilha[i - 1];

        fprintf(arqsvg, "%lf,%lf ", g->vertices[u].x, g->vertices[u].y);
    }
    char idCaminho[20];
    snprintf(idCaminho, sizeof(idCaminho), "%d%d%d%d", r->criterio, r->origem, r->destino, rand());

    fprintf(arqsvg, "%lf,%lf ", g->vertices[r->destino].x, g->vertices[r->destino].y);

    fprintf(arqsvg, "\" stroke=\"%s\" stroke-width=\"3\" fill=\"none\" id=\"%s\"/>\n", cor, idCaminho);

    fprintf(arqsvg, "<text x=\"%lf\" y=\"%lf\" text-anchor=\"middle\" font-weight=\"bold\" >%s</text>\n", g->vertices[r->destino].x, g->vertices[r->destino].y - 10, "F");

    fprintf(arqsvg, "<circle r=\"20\" fill=\"%s\" >\n", cor);

    fprintf(arqsvg, "<animateMotion dur=\"%lfs\" repeatCount=\"indefinite\">\n", r->tempoTotal);

    fprintf(arqsvg, "<mpath href=\"#%s\" />\n", idCaminho);

    fprintf(arqsvg, "</animateMotion>\n");

    fprintf(arqsvg, "</circle>");

    free(pilha);
}

void alteraVelocidade(Grafo g, double novaVelocidade, double x, double y, double w, double h) {
    if (g == NULL) {
        return;
    }

    GrafoInterno grafo = (GrafoInterno) g;

    for (int i = 0; i < grafo->qtdVertices; i++) {
        Aresta atual = grafo->vertices[i].listaAdj;

        while (atual != NULL) {
            int destino = atual->destino;
            if( (grafo->vertices[i].x >= x && grafo->vertices[i].x <= x + w && grafo->vertices[i].y >= y && grafo->vertices[i].y <= y + h) &&
                (grafo->vertices[destino].x >= x && grafo->vertices[destino].x <= x + w && grafo->vertices[destino].y >= y && grafo->vertices[destino].y <= y + h)) {
                atual->velocidade = novaVelocidade;
            }
            atual = atual->prox;
        }
    }
}


struct sTarjan {
    int *indice;
    int *menorIndice;
    int *naPilha;
    int *pilha;
    int *componente;

    int topo;
    int proximoIndice;
    int qtdComponentes;

    double limiteVelocidade;
};

static void geraCorComponente(int indice, char *cor, size_t tamanho) {
    unsigned int s0 = (unsigned int)(indice + 1) * 1103515245u + 12345u;
    unsigned int s1 = (s0 ^ 0x9e3779b9u) * 2654435761u;
    unsigned int s2 = (s1 ^ (s1 >> 16)) * 2246822519u;
    unsigned int s3 = (s2 ^ (s2 >> 13)) * 3266489917u;
    unsigned int valor = s3 ^ (s3 >> 16);

    unsigned int r = (valor >> 24) & 0xFFu;
    unsigned int g = (valor >> 16) & 0xFFu;
    unsigned int b = (valor >> 8) & 0xFFu;

    snprintf(cor, tamanho, "rgb(%u,%u,%u)", r, g, b);
}

static void tarjanVisita(
    GrafoInterno g,
    int u,
    struct sTarjan *dados
) {
    dados->indice[u] = dados->proximoIndice;
    dados->menorIndice[u] = dados->proximoIndice;
    dados->proximoIndice++;

    dados->pilha[dados->topo] = u;
    dados->topo++;

    dados->naPilha[u] = 1;

    Aresta aresta = g->vertices[u].listaAdj;

    while (aresta != NULL) {
        /*
         * Considera apenas arestas direcionadas u -> v
         * cuja velocidade é inferior a vl.
         */
        if (aresta->velocidade < dados->limiteVelocidade) {
            int v = aresta->destino;

            if (dados->indice[v] == -1) {
                tarjanVisita(g, v, dados);

                if (dados->menorIndice[v] < dados->menorIndice[u]) {
                    dados->menorIndice[u] = dados->menorIndice[v];
                }
            } else if (dados->naPilha[v]) {
                if (dados->indice[v] < dados->menorIndice[u]) {
                    dados->menorIndice[u] = dados->indice[v];
                }
            }
        }

        aresta = aresta->prox;
    }

    /*
     * Se u é raiz de um componente fortemente conexo,
     * desempilha todos os vértices desse componente.
     */
    if (dados->menorIndice[u] == dados->indice[u]) {
        int v;

        do {
            dados->topo--;
            v = dados->pilha[dados->topo];

            dados->naPilha[v] = 0;
            dados->componente[v] = dados->qtdComponentes;

        } while (v != u);

        dados->qtdComponentes++;
    }
}

int *calculaComponentesFortesVelocidade(
    Grafo grafo,
    double vl,
    int *qtdComponentes
) {
    if (grafo == NULL || qtdComponentes == NULL) {
        return NULL;
    }

    GrafoInterno g = (GrafoInterno) grafo;

    int n = g->qtdVertices;

    *qtdComponentes = 0;

    if (n == 0) {
        return NULL;
    }

    struct sTarjan dados;

    dados.indice = malloc(n * sizeof(int));
    dados.menorIndice = malloc(n * sizeof(int));
    dados.naPilha = malloc(n * sizeof(int));
    dados.pilha = malloc(n * sizeof(int));
    dados.componente = malloc(n * sizeof(int));

    if (
        dados.indice == NULL ||
        dados.menorIndice == NULL ||
        dados.naPilha == NULL ||
        dados.pilha == NULL ||
        dados.componente == NULL
    ) {
        free(dados.indice);
        free(dados.menorIndice);
        free(dados.naPilha);
        free(dados.pilha);
        free(dados.componente);

        return NULL;
    }

    dados.topo = 0;
    dados.proximoIndice = 0;
    dados.qtdComponentes = 0;
    dados.limiteVelocidade = vl;

    for (int i = 0; i < n; i++) {
        dados.indice[i] = -1;
        dados.menorIndice[i] = -1;
        dados.naPilha[i] = 0;
        dados.componente[i] = -1;
    }

    for (int i = 0; i < n; i++) {
        if (dados.indice[i] == -1) {
            tarjanVisita(g, i, &dados);
        }
    }

    *qtdComponentes = dados.qtdComponentes;

    free(dados.indice);
    free(dados.menorIndice);
    free(dados.naPilha);
    free(dados.pilha);

    /*
     * Não libera dados.componente porque ele será retornado.
     */
    return dados.componente;
}

int imprimeComponentesFortesVelocidade(
    Grafo grafo,
    double vl,
    FILE *arqsvg
) {
    if (grafo == NULL) {
        return -1;
    }

    int i = 0;

    GrafoInterno g = (GrafoInterno) grafo;

    int qtdComponentes = 0;

    int *componentes = calculaComponentesFortesVelocidade(
        grafo,
        vl,
        &qtdComponentes
    );

    if (componentes == NULL) {
        printf("Erro ao calcular componentes fortemente conexos.\n");
        return -1;
    }

    printf(
        "Quantidade de componentes fortemente conexos com velocidade inferior a %.2lf: %d\n",
        vl,
        qtdComponentes
    );

    for (int c = 0; c < qtdComponentes; c++) {
        int quantidadeVertices = 0;
        int primeiroVertice = 1;

        double minX = 0;
        double minY = 0;
        double maxX = 0;
        double maxY = 0;

        printf("Componente %d:", c);

        for (int i = 0; i < g->qtdVertices; i++) {
            if (componentes[i] == c) {
                double x = g->vertices[i].x;
                double y = g->vertices[i].y;

                printf(" %s", g->vertices[i].id);

                quantidadeVertices++;

                if (primeiroVertice) {
                    minX = x;
                    maxX = x;
                    minY = y;
                    maxY = y;

                    primeiroVertice = 0;
                } else {
                    if (x < minX) {
                        minX = x;
                    }

                    if (x > maxX) {
                        maxX = x;
                    }

                    if (y < minY) {
                        minY = y;
                    }

                    if (y > maxY) {
                        maxY = y;
                    }
                }
            }
        }

        printf("\n");

        if (quantidadeVertices > 1) {
            double largura = maxX - minX;
            double altura = maxY - minY;

            if(largura == 0) {
                largura = 10;
            }

            if(altura == 0) {
                altura = 10;
            }

            printf(
                "  Bounding box: x=%.2lf y=%.2lf largura=%.2lf altura=%.2lf\n",
                minX,
                minY,
                largura,
                altura
            );

            printf(
                "  Limites: (%.2lf, %.2lf) ate (%.2lf, %.2lf)\n",
                minX,
                minY,
                maxX,
                maxY
            );

            if (arqsvg != NULL && largura > 0 && altura > 0) {
                char corComponente[32];
                geraCorComponente(c, corComponente, sizeof(corComponente));

                fprintf(
                    arqsvg,
                    "  <rect x=\"%.2lf\" y=\"%.2lf\" width=\"%.2lf\" height=\"%.2lf\" "
                    "fill=\"%s\" fill-opacity=\"0.5\" stroke=\"black\" stroke-width=\"1\"/>\n",
                    minX,
                    minY,
                    largura,
                    altura,
                    corComponente
                );
                i++;
            }
        } else {
            printf("  Componente unitario: bounding box nao gerado.\n");
        }

        printf("\n");
    }

    free(componentes);

    return i;
}


struct sArestaAGM {
    int origem;
    int destino;

    double comprimento;
    double velocidade;
    double peso;

    char cepDir[TAM_CEP];
    char cepEsq[TAM_CEP];
    char nomeRua[TAM_RUA];

    Aresta original;
};

struct sArvoreGeradora {
    struct sArestaAGM *arestas;

    int qtdVertices;
    int qtdArestas;

    int conexa;

    double custoTotal;
    double limiteVelocidade;

};

typedef struct sArvoreGeradora *ArvoreGeradoraInterna;

struct sUnionFind {
    int *pai;
    int *rank;
    int n;
};

typedef struct sUnionFind *UnionFind;

static UnionFind criaUnionFind(int n) {
    UnionFind uf = malloc(sizeof(struct sUnionFind));

    if (uf == NULL) {
        return NULL;
    }

    uf->pai = malloc(n * sizeof(int));
    uf->rank = malloc(n * sizeof(int));

    if (uf->pai == NULL || uf->rank == NULL) {
        free(uf->pai);
        free(uf->rank);
        free(uf);
        return NULL;
    }

    uf->n = n;

    for (int i = 0; i < n; i++) {
        uf->pai[i] = i;
        uf->rank[i] = 0;
    }

    return uf;
}

static int findUnionFind(UnionFind uf, int x) {
    if (uf->pai[x] != x) {
        uf->pai[x] = findUnionFind(uf, uf->pai[x]);
    }

    return uf->pai[x];
}

static int unionUnionFind(UnionFind uf, int a, int b) {
    int raizA = findUnionFind(uf, a);
    int raizB = findUnionFind(uf, b);

    if (raizA == raizB) {
        return 0;
    }

    if (uf->rank[raizA] < uf->rank[raizB]) {
        uf->pai[raizA] = raizB;
    } else if (uf->rank[raizA] > uf->rank[raizB]) {
        uf->pai[raizB] = raizA;
    } else {
        uf->pai[raizB] = raizA;
        uf->rank[raizA]++;
    }

    return 1;
}

static void liberaUnionFind(UnionFind uf) {
    if (uf == NULL) {
        return;
    }

    free(uf->pai);
    free(uf->rank);
    free(uf);
}

static int comparaArestaAGM(
    const void *a,
    const void *b
) {
    const struct sArestaAGM *e1 = a;
    const struct sArestaAGM *e2 = b;

    if (e1->peso < e2->peso) {
        return -1;
    }

    if (e1->peso > e2->peso) {
        return 1;
    }

    return 0;
}

ArvoreGeradora calculaArvoreGeradoraMinimaVelocidade(
    Grafo grafo,
    double vl
) {
    if (grafo == NULL) {
        return NULL;
    }

    GrafoInterno g = (GrafoInterno) grafo;

    int n = g->qtdVertices;

    if (n == 0) {
        return NULL;
    }

    struct sArestaAGM *candidatas =
        malloc(g->qtdArestas * sizeof(struct sArestaAGM));

    if (candidatas == NULL) {
        return NULL;
    }

    int qtdCandidatas = 0;

    for (int u = 0; u < n; u++) {
        Aresta atual = g->vertices[u].listaAdj;

        while (atual != NULL) {
            if (atual->velocidade < vl) {
                candidatas[qtdCandidatas].origem = u;
                candidatas[qtdCandidatas].destino = atual->destino;

                candidatas[qtdCandidatas].comprimento = atual->comprimento;
                candidatas[qtdCandidatas].velocidade = atual->velocidade;

                /*
                 * Critério da árvore mínima:
                 * entre as arestas abaixo de vl,
                 * escolhe as de menor comprimento.
                 */
                candidatas[qtdCandidatas].peso = atual->comprimento;

                copiaString(
                    candidatas[qtdCandidatas].cepDir,
                    atual->cepDir,
                    TAM_CEP
                );

                copiaString(
                    candidatas[qtdCandidatas].cepEsq,
                    atual->cepEsq,
                    TAM_CEP
                );

                copiaString(
                    candidatas[qtdCandidatas].nomeRua,
                    atual->nomeRua,
                    TAM_RUA
                );

                /*
                 * Guarda o ponteiro para a aresta original,
                 * para depois alterar sua velocidade.
                 */
                candidatas[qtdCandidatas].original = atual;

                qtdCandidatas++;
            }

            atual = atual->prox;
        }
    }

    qsort(
        candidatas,
        qtdCandidatas,
        sizeof(struct sArestaAGM),
        comparaArestaAGM
    );

    ArvoreGeradoraInterna agm =
        malloc(sizeof(struct sArvoreGeradora));

    if (agm == NULL) {
        free(candidatas);
        return NULL;
    }

    agm->arestas = malloc((n - 1) * sizeof(struct sArestaAGM));

    if (agm->arestas == NULL) {
        free(candidatas);
        free(agm);
        return NULL;
    }

    agm->qtdVertices = n;
    agm->qtdArestas = 0;
    agm->conexa = 0;
    agm->custoTotal = 0;
    agm->limiteVelocidade = vl;

    UnionFind uf = criaUnionFind(n);

    if (uf == NULL) {
        free(candidatas);
        free(agm->arestas);
        free(agm);
        return NULL;
    }

    for (int i = 0; i < qtdCandidatas; i++) {
        int u = candidatas[i].origem;
        int v = candidatas[i].destino;

        if (unionUnionFind(uf, u, v)) {
            /*
             * Adiciona a aresta na AGM.
             */
            agm->arestas[agm->qtdArestas] = candidatas[i];
            agm->qtdArestas++;

            agm->custoTotal += candidatas[i].peso;

            /*
             * Aumenta a velocidade da aresta original em 50%.
             */
            if (candidatas[i].original != NULL) {
                candidatas[i].original->velocidade *= 1.5;
            }

            /*
             * Atualiza também a cópia guardada na AGM,
             * para impressão sair com o valor novo.
             */
            agm->arestas[agm->qtdArestas - 1].velocidade *= 1.5;

            if (agm->qtdArestas == n - 1) {
                break;
            }
        }
    }

    if (agm->qtdArestas == n - 1) {
        agm->conexa = 1;
    } else {
        agm->conexa = 0;
    }

    liberaUnionFind(uf);
    free(candidatas);

    return agm;
}


double getCustoArvoreGeradora(
    ArvoreGeradora arvore
) {
    if (arvore == NULL) {
        return 0;
    }

    ArvoreGeradoraInterna agm = (ArvoreGeradoraInterna) arvore;

    return agm->custoTotal;
}

int getQuantidadeArestasArvore(
    ArvoreGeradora arvore
) {
    if (arvore == NULL) {
        return 0;
    }

    ArvoreGeradoraInterna agm = (ArvoreGeradoraInterna) arvore;

    return agm->qtdArestas;
}

int arvoreGeradoraEhConexa(
    ArvoreGeradora arvore
) {
    if (arvore == NULL) {
        return 0;
    }

    ArvoreGeradoraInterna agm = (ArvoreGeradoraInterna) arvore;

    return agm->conexa;
}

void liberaArvoreGeradora(
    ArvoreGeradora arvore
) {
    if (arvore == NULL) {
        return;
    }

    ArvoreGeradoraInterna agm = (ArvoreGeradoraInterna) arvore;

    free(agm->arestas);
    free(agm);
}

void imprimeArvoreGeradoraMinima(
    Grafo grafo,
    ArvoreGeradora arvore,
    FILE *arqsvg
) {
    if (grafo == NULL || arvore == NULL) {
        return;
    }

    GrafoInterno g = (GrafoInterno) grafo;
    ArvoreGeradoraInterna agm = (ArvoreGeradoraInterna) arvore;

    printf("\nArvore Geradora Minima\n");

    printf(
        "Limite de velocidade: %.2lf\n",
        agm->limiteVelocidade
    );

    if (agm->conexa) {
        printf("Resultado: arvore geradora conexa\n");
    } else {
        printf("Resultado: grafo desconexo, foi gerada uma floresta\n");
    }

    printf("Arestas selecionadas: %d\n", agm->qtdArestas);
    printf("Custo total: %.4lf\n\n", agm->custoTotal);

    for (int i = 0; i < agm->qtdArestas; i++) {
        int u = agm->arestas[i].origem;
        int v = agm->arestas[i].destino;

        printf(
            "%s -- %s | rua: %s | comp: %.2lf | vel: %.2lf | peso: %.4lf\n",
            g->vertices[u].id,
            g->vertices[v].id,
            agm->arestas[i].nomeRua,
            agm->arestas[i].comprimento,
            agm->arestas[i].velocidade,
            agm->arestas[i].peso
        );

        if (arqsvg != NULL) {
            fprintf(
                arqsvg,
                "<line x1=\"%.2lf\" y1=\"%.2lf\" "
                "x2=\"%.2lf\" y2=\"%.2lf\" "
                "stroke=\"red\" stroke-width=\"2\" />\n",
                g->vertices[u].x,
                g->vertices[u].y,
                g->vertices[v].x,
                g->vertices[v].y
            );
        }
    }

    printf("\n");
}