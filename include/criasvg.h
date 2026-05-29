#ifndef CRIASVG
#define CRIASVG
#include "hashfile.h"
#include "quadra.h"
#include <stdio.h>


/// @brief Desenha uma quadra no arquivo SVG
/// @param q quadra a ser desenhada
/// @param arq arquivo SVG a ser desenhado
void desenhaSVGQuadra(Quadra q, FILE *arq);

/// @brief Cria um X vermelho no local da quadra
/// @param q quadra onde o X será criado
/// @param arq arquivo SVG onde o X será criado
void desenhaXvermelho(Quadra q, FILE *arq);

#endif