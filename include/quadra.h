#ifndef QUA_H
#define QUA_H
#include <stdio.h>

//programa para quadras.

typedef void *Quadra;

Quadra criaQuadra(const char *cep, double x, double y, double w, double h, const char *sw, const char *cfill, const char *cstrk);
//cria a quadra com suas informações

void destruirQuadra(Quadra q);
//destrói a quadra

//getters
char* getCep(Quadra q);
double getX(Quadra q);
double getY(Quadra q);
double getW(Quadra q);
double getH(Quadra q);
char* getSw(Quadra q);
char* getCfill(Quadra q);
char* getCstrk(Quadra q);

size_t tamanhoQuadra(void);
//retorna o tamanho da quadra

void printQuadra(Quadra q);
//imprime a quadra no formato

int calculaPontoEndereco(
    Quadra q,
    char face,
    double numero,
    double *px,
    double *py
);

#endif