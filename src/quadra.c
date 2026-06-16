#include "quadra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct sQuadra {
    char cep[10];
    double x, y, w, h;
    char sw[10], cfill[50], cstrk[50];
};

typedef struct sQuadra sQuadra;

size_t tamanhoQuadra(void) {
    return sizeof(sQuadra);
}

Quadra criaQuadra(const char *cep, double x, double y, double w, double h, const char *sw, const char *cfill, const char *cstrk) {
    sQuadra* nova = (sQuadra*)malloc(sizeof(sQuadra));
    if (!nova) {
        printf("Erro na alocação de quadra\n");
        return NULL;
    }
    strcpy(nova->cep, cep);
    nova->x = x;
    nova->y = y;
    nova->w = w;
    nova->h = h;
    strcpy(nova->sw, sw);
    strcpy(nova->cfill, cfill);
    strcpy(nova->cstrk, cstrk);
    return nova;
}

void destruirQuadra(Quadra q) {
    free(q);
}

char* getCep(Quadra q) {
    sQuadra* quadra = (sQuadra*)q;
    return quadra->cep;
}

double getX(Quadra q) {
    sQuadra* quadra = (sQuadra*)q;
    return quadra->x;
}

double getY(Quadra q) {
    sQuadra* quadra = (sQuadra*)q;
    return quadra->y;
}

double getW(Quadra q) {
    sQuadra* quadra = (sQuadra*)q;
    return quadra->w;
}

double getH(Quadra q) {
    sQuadra* quadra = (sQuadra*)q;
    return quadra->h;
}

char* getSw(Quadra q) {
    sQuadra* quadra = (sQuadra*)q;
    return quadra->sw;
}

char* getCfill(Quadra q) {
    sQuadra* quadra = (sQuadra*)q;
    return quadra->cfill;
}

char* getCstrk(Quadra q) {
    sQuadra* quadra = (sQuadra*)q;
    return quadra->cstrk;
}

void printQuadra(Quadra q) {
    sQuadra* quadra = (sQuadra*)q;
    printf("Quadra: CEP=%s, X=%.2f, Y=%.2f, W=%.2f, H=%.2f, SW=%s, Cfill=%s, Cstrk=%s\n",
           quadra->cep, quadra->x, quadra->y, quadra->w, quadra->h,
           quadra->sw, quadra->cfill, quadra->cstrk);
}

int calculaPontoEndereco(
    Quadra quadra,
    char face,
    double numero,
    double *px,
    double *py
) {
    if (quadra == NULL || px == NULL || py == NULL) {
        return 0;
    }

    struct sQuadra *q = (struct sQuadra *) quadra;

    switch (face) {
        case 'S':
            *px = q->x + q->w - numero;
            *py = q->y + q->h;
            break;

        case 'L':
            *px = q->x + q->w;
            *py = q->y + q->h - numero;
            break;

        case 'N':
            *px = q->x + q->w - numero;
            *py = q->y;
            break;

        case 'O':
            *px = q->x;
            *py = q->y + q->h - numero;
            break;

        default:
            return 0;
    }

    return 1;
}