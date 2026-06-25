#ifndef CRIASVG
#define CRIASVG
#include "hashfile.h"
#include "quadra.h"

/// @brief Desenha uma quadra no arquivo SVG
/// @param q quadra a ser desenhada
/// @param arq arquivo SVG a ser desenhado
void desenhaSVGQuadra(Quadra q, FILE *arq);

/// @brief Cria um X vermelho no local da quadra
/// @param q quadra onde o X será criado
/// @param arq arquivo SVG onde o X será criado
void desenhaXvermelho(Quadra q, FILE *arq);

/// @brief Cria uma linha pontilhada no arquivo SVG
/// @param x coordenada x do ponto inicial da linha
/// @param y coordenada y do ponto inicial da linha
/// @param arq arquivo SVG onde a linha será criada
/// @param reg nome do registrador associado à linha
void crialinhapontilhada(double x, double y, FILE *arq, char *reg);

void crialinha(double x1, double y1, double x2, double y2, FILE *arq, char *cor);
/// @brief Cria uma linha no arquivo SVG
/// @param x1 coordenada x do ponto inicial da linha
/// @param y1 coordenada y do ponto inicial da linha
/// @param x2 coordenada x do ponto final da linha
/// @param y2 coordenada y do ponto final da linha
/// @param arq arquivo SVG onde a linha será criada
/// @param cor cor da linha

#endif