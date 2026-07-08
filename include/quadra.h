#ifndef QUA_H
#define QUA_H
#include <stdio.h>

//programa para quadras.

typedef void *Quadra;

/// @brief Cria uma quadra
/// @param cep CEP da quadra
/// @param x coordenada x do canto superior esquerdo
/// @param y coordenada y do canto superior esquerdo
/// @param w largura da quadra
/// @param h altura da quadra
/// @param sw espessura da borda
/// @param cfill cor de preenchimento
/// @param cstrk cor da borda
/// @return ponteiro para a quadra criada
Quadra criaQuadra(const char *cep, double x, double y, double w, double h, const char *sw, const char *cfill, const char *cstrk);

/// @brief Destrói uma quadra
/// @param q quadra a ser destruída
void destruirQuadra(Quadra q);

//getters
char* getCep(Quadra q);
double getX(Quadra q);
double getY(Quadra q);
double getW(Quadra q);
double getH(Quadra q);
char* getSw(Quadra q);
char* getCfill(Quadra q);
char* getCstrk(Quadra q);

/// @brief Retorna o tamanho da estrutura sQuadra
/// @return tamanho da estrutura
size_t tamanhoQuadra(void);

/// @brief Imprime as informações de uma quadra
/// @param q quadra a ser impressa
void printQuadra(Quadra q);

/// @brief Calcula o ponto de endereço de uma quadra
/// @param q quadra
/// @param face face da quadra
/// @param numero número do endereço
/// @param px ponteiro para a coordenada x do ponto
/// @param py ponteiro para a coordenada y do ponto
/// @return 1 se o cálculo foi bem-sucedido, 0 caso contrário
int calculaPontoEndereco(
    Quadra q,
    char face,
    double numero,
    double *px,
    double *py
);

#endif