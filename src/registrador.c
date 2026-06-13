#include "registrador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_REG 16
#define TAM_CEP 20

struct sRegistrador {
    int indice;

    char id[TAM_REG];

    char cep[TAM_CEP];
    char face;
    double numero;

    double x;
    double y;

    int verticeMaisProximo;
};

struct sRegistradores {
    struct sRegistrador *vetor;
    int capacidade;
};




Registradores criaRegistradores(int capacidade) {
    struct sRegistradores *regs = malloc(sizeof(struct sRegistradores));
    regs->vetor = malloc(capacidade * sizeof(struct sRegistrador));
    regs->capacidade = capacidade;
    return (Registradores) regs;
}

static int idRegistradorParaIndice(const char *idReg) {
    int indice;

    if (sscanf(idReg, "R%d", &indice) != 1) {
        return -1;
    }

    return indice;
}

int adicionaRegistrador(
    Registradores registradores,
    const char *idReg,
    const char *cep,
    char face,
    double numero,
    double x,
    double y,
    int verticeMaisProximo
) {
    if (registradores == NULL || idReg == NULL || cep == NULL) {
        return 0;
    }

    struct sRegistradores *regs = (struct sRegistradores *) registradores;

    int indice = idRegistradorParaIndice(idReg);

    if (indice < 0 || indice >= regs->capacidade) {
        return 0;
    }

    regs->vetor[indice].indice = indice;

    strncpy(regs->vetor[indice].id, idReg, TAM_REG - 1);
    regs->vetor[indice].id[TAM_REG - 1] = '\0';

    strncpy(regs->vetor[indice].cep, cep, TAM_CEP - 1);
    regs->vetor[indice].cep[TAM_CEP - 1] = '\0';

    regs->vetor[indice].face = face;
    regs->vetor[indice].numero = numero;

    regs->vetor[indice].x = x;
    regs->vetor[indice].y = y;

    regs->vetor[indice].verticeMaisProximo = verticeMaisProximo;

    return 1;
}


int buscaRegistrador(
    Registradores regs,
    const char *idReg
) {
    if (regs == NULL || idReg == NULL) {
        return 0;
    }

    struct sRegistradores *registradores = (struct sRegistradores *) regs;

    int indice = idRegistradorParaIndice(idReg);

    if (indice < 0 || indice >= registradores->capacidade) {
        return 0;
    }

    return 1;
}

int getVerticeRegistrador(
    Registradores regs,
    const char *idReg
) {
    if (regs == NULL || idReg == NULL) {
        return -1;
    }

    struct sRegistradores *registradores = (struct sRegistradores *) regs;

    int indice = idRegistradorParaIndice(idReg);

    if (indice < 0 || indice >= registradores->capacidade) {
        return -1;
    }

    return registradores->vetor[indice].verticeMaisProximo;
}

double getXRegistrador(
    Registradores regs,
    const char *idReg
) {
    if (regs == NULL || idReg == NULL) {
        return 0;
    }

    struct sRegistradores *registradores = (struct sRegistradores *) regs;

    int indice = idRegistradorParaIndice(idReg);

    if (indice < 0 || indice >= registradores->capacidade) {
        return 0;
    }

    return registradores->vetor[indice].x;
}

double getYRegistrador(
    Registradores regs,
    const char *idReg
) {
    if (regs == NULL || idReg == NULL) {
        return 0;
    }

    struct sRegistradores *registradores = (struct sRegistradores *) regs;

    int indice = idRegistradorParaIndice(idReg);

    if (indice < 0 || indice >= registradores->capacidade) {
        return 0;
    }

    return registradores->vetor[indice].y;
}

char *getCepRegistrador(
    Registradores regs,
    const char *idReg
) {
    if (regs == NULL || idReg == NULL) {
        return NULL;
    }

    struct sRegistradores *registradores = (struct sRegistradores *) regs;

    int indice = idRegistradorParaIndice(idReg);

    if (indice < 0 || indice >= registradores->capacidade) {
        return NULL;
    }

    return registradores->vetor[indice].cep;
}

char getFaceRegistrador(
    Registradores regs,
    const char *idReg
) {
    if (regs == NULL || idReg == NULL) {
        return '\0';
    }

    struct sRegistradores *registradores = (struct sRegistradores *) regs;

    int indice = idRegistradorParaIndice(idReg);

    if (indice < 0 || indice >= registradores->capacidade) {
        return '\0';
    }

    return registradores->vetor[indice].face;
}

double getNumeroRegistrador(
    Registradores regs,
    const char *idReg
) {
    if (regs == NULL || idReg == NULL) {
        return 0;
    }

    struct sRegistradores *registradores = (struct sRegistradores *) regs;

    int indice = idRegistradorParaIndice(idReg);

    if (indice < 0 || indice >= registradores->capacidade) {
        return 0;
    }

    return registradores->vetor[indice].numero;
}

void printRegistradores(Registradores regs) {
    if (regs == NULL) {
        return;
    }

    struct sRegistradores *registradores = (struct sRegistradores *) regs;

    printf("Registradores:\n");
    for (int i = 0; i < registradores->capacidade; i++) {
        if (registradores->vetor[i].id[0] != '\0') {
            printf("ID: %s, CEP: %s, Face: %c, Numero: %.2f, X: %.2f, Y: %.2f, VerticeMaisProximo: %d\n",
                   registradores->vetor[i].id,
                   registradores->vetor[i].cep,
                   registradores->vetor[i].face,
                   registradores->vetor[i].numero,
                   registradores->vetor[i].x,
                   registradores->vetor[i].y,
                   registradores->vetor[i].verticeMaisProximo);
        }
    }
}