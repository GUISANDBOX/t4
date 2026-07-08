#ifndef GRAFO_H
#define GRAFO_H
#define INF 1e100
#include <stdio.h>

// Estrutura de dados para representar um grafo

// Definição de tipos opacos para o grafo e o resultado do algoritmo de Dijkstra
typedef void *Grafo;
typedef void *ResultadoDijkstra;

// Enumeração para os critérios de Dijkstra
typedef enum {
    MENOR_DISTANCIA, // 0
    MENOR_TEMPO // 1
} CriterioDijkstra;

/// @brief Cria um grafo com capacidade especificada
/// @param capacidadeVertices capacidade dos vértices do grafo
/// @return ponteiro para o grafo criado
Grafo criaGrafo(int capacidadeVertices);

/// @brief Adiciona um vértice ao grafo
/// @param g ponteiro para o grafo
/// @param id identificador do vértice
/// @param x coordenada x do vértice
/// @param y coordenada y do vértice
/// @return 1 se o vértice foi adicionado com sucesso, 0 caso contrário
int adicionaVertice(Grafo g, const char *id, double x, double y);

/// @brief Adiciona uma aresta ao grafo
/// @param g ponteiro para o grafo
/// @param idOrigem identificador do vértice de origem
/// @param idDestino identificador do vértice de destino
/// @param cepDir CEP da direita da aresta
/// @param cepEsq CEP da esquerda da aresta
/// @param comprimento comprimento da aresta
/// @param velocidade velocidade na aresta
/// @param nomeRua nome da rua da aresta
/// @return 1 se a aresta foi adicionada com sucesso, 0 caso contrário
int adicionaAresta(Grafo g, const char *idOrigem, const char *idDestino,
                   const char *cepDir, const char *cepEsq, double comprimento,
                   double velocidade, const char *nomeRua);

/// @brief Imprime o grafo
/// @param g ponteiro para o grafo
void imprimeGrafo(Grafo g);

/// @brief Libera a memória alocada para o grafo
/// @param g ponteiro para o grafo
void liberaGrafo(Grafo g);

/// @brief Busca o índice de um vértice no grafo pelo seu identificador
/// @param g ponteiro para o grafo
/// @param id identificador do vértice
/// @return índice do vértice se encontrado, -1 caso contrário
int buscaIndiceVertice(Grafo g, const char *id);

/// @brief Encontra o índice do vértice mais próximo de uma coordenada
/// @param g ponteiro para o grafo
/// @param x coordenada x
/// @param y coordenada y
/// @return índice do vértice mais próximo se encontrado, -1 caso contrário
int verticeMaisProximo(
    Grafo g,
    double x,
    double y
);

/// @brief Retorna a quantidade de vértices no grafo
/// @param g ponteiro para o grafo
/// @return quantidade de vértices
int getQuantidadeVertices(Grafo g);

/// @brief Retorna a quantidade de arestas no grafo
/// @param g ponteiro para o grafo
/// @return quantidade de arestas
int getQuantidadeArestas(Grafo g);

/// @brief Retorna o identificador de um vértice pelo seu índice
/// @param g ponteiro para o grafo
/// @param indice índice do vértice
/// @return identificador do vértice se encontrado, NULL caso contrário
char *getIdVertice(Grafo g, int indice);

/// @brief Retorna a coordenada x de um vértice pelo seu índice
/// @param g ponteiro para o grafo
/// @param indice índice do vértice
/// @return coordenada x se encontrado, 0 caso contrário
double getXVertice(Grafo g, int indice);

/// @brief Retorna a coordenada y de um vértice pelo seu índice
/// @param g ponteiro para o grafo
/// @param indice índice do vértice
/// @return coordenada y se encontrado, 0 caso contrário
double getYVertice(Grafo g, int indice);

/// @brief Calcula o caminho mínimo entre dois vértices
/// @param g ponteiro para o grafo
/// @param origem índice do vértice de origem
/// @param destino índice do vértice de destino
/// @param criterio critério para a busca
/// @return resultado da busca
ResultadoDijkstra dijkstra(
    Grafo g,
    int origem,
    int destino,
    CriterioDijkstra criterio
);

/// @brief Imprime o caminho mínimo encontrado
/// @param g ponteiro para o grafo
/// @param resultado resultado da busca
/// @param arqtxt ponteiro para o arquivo de saída
void imprimeCaminho(
    Grafo g,
    ResultadoDijkstra resultado,
    FILE *arqtxt
);

/// @brief Retorna o custo do caminho mínimo encontrado
/// @param resultado resultado da busca
/// @return custo do caminho mínimo
double getCustoResultado(
    ResultadoDijkstra resultado
);

/// @brief Verifica se existe um caminho entre dois vértices
/// @param resultado resultado da busca
/// @return 1 se existir caminho, 0 caso contrário
int existeCaminho(
    ResultadoDijkstra resultado
);

/// @brief Libera a memória alocada para o resultado da busca
/// @param resultado resultado da busca
void liberaResultadoDijkstra(
    ResultadoDijkstra resultado
);

/// @brief Escreve o caminho mínimo encontrado em um arquivo SVG
/// @param g ponteiro para o grafo
/// @param resultado resultado da busca
/// @param cor cor do caminho
/// @param arqsvg ponteiro para o arquivo SVG
void escreveCaminhoSVG(Grafo g, ResultadoDijkstra resultado, char* cor, FILE *arqsvg);

/// @brief Altera a velocidade das arestas dentro de uma região retangular definida por (x, y, w, h)
/// @param g ponteiro para o grafo
/// @param novaVelocidade nova velocidade das arestas
/// @param x coordenada x do canto superior esquerdo da região
/// @param y coordenada y do canto superior esquerdo da região
/// @param w largura da região
/// @param h altura da região
void alteraVelocidade(Grafo g, double novaVelocidade, double x, double y, double w, double h);

/// @brief Calcula os componentes fortemente conectados do grafo considerando apenas arestas com velocidade maior ou igual a vl
/// @param g ponteiro para o grafo
/// @param vl velocidade mínima
/// @param qtdComponentes ponteiro para a quantidade de componentes
/// @return ponteiro para o array de componentes
int *calculaComponentesFortesVelocidade(
    Grafo g,
    double vl,
    int *qtdComponentes
);

/// @brief Imprime os componentes fortemente conectados do grafo considerando apenas arestas com velocidade maior ou igual a vl
/// @param g ponteiro para o grafo
/// @param vl velocidade mínima
/// @param arqsvg ponteiro para o arquivo SVG
/// @return quantidade de componentes fortemente conectados encontrados, -1 em caso de erro
int imprimeComponentesFortesVelocidade(
    Grafo g,
    double vl,
    FILE *arqsvg
);

/// Estrutura de dados para representar uma árvore geradora mínima
typedef void *ArvoreGeradora;

/// @brief Calcula a árvore geradora mínima do grafo considerando apenas arestas com velocidade menor que vl
/// @param g ponteiro para o grafo
/// @param vl velocidade máxima
/// @return ponteiro para a árvore geradora mínima
ArvoreGeradora calculaArvoreGeradoraMinimaVelocidade(
    Grafo g,
    double vl
);

/// @brief Imprime a árvore geradora mínima
/// @param g ponteiro para o grafo
/// @param agm ponteiro para a árvore geradora mínima
/// @param arqsvg ponteiro para o arquivo SVG
void imprimeArvoreGeradoraMinima(
    Grafo g,
    ArvoreGeradora agm,
    FILE *arqsvg
);

/// @brief Libera a memória alocada para a árvore geradora mínima
/// @param agm ponteiro para a árvore geradora mínima
void liberaArvoreGeradora(
    ArvoreGeradora agm
);

#endif