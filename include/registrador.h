#ifndef REGISTRADOR_H
#define REGISTRADOR_H

typedef void *Registradores;

Registradores criaRegistradores(int capacidade);

int adicionaRegistrador(
    Registradores regs,
    const char *idReg,
    const char *cep,
    char face,
    double numero,
    double x,
    double y,
    int verticeMaisProximo
);

int buscaRegistrador(
    Registradores regs,
    const char *idReg
);

int getVerticeRegistrador(
    Registradores regs,
    const char *idReg
);

double getXRegistrador(
    Registradores regs,
    const char *idReg
);

double getYRegistrador(
    Registradores regs,
    const char *idReg
);

char *getCepRegistrador(
    Registradores regs,
    const char *idReg
);

char getFaceRegistrador(
    Registradores regs,
    const char *idReg
);

double getNumeroRegistrador(
    Registradores regs,
    const char *idReg
);


void printRegistradores(Registradores regs);

#endif