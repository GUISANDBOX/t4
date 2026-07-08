#ifndef REGISTRADOR_H
#define REGISTRADOR_H

//Programa de TAD para representar registradores.

/// @brief Possui registradores - definir como uma struct.
typedef void *Registradores;

/// @brief Cria registradores com capacidade especificada
/// @param capacidade capacidade dos registradores
/// @return ponteiro para os registradores criados
Registradores criaRegistradores(int capacidade);

/// @brief Adiciona um registrador aos registradores
/// @param regs ponteiro para os registradores
/// @param idReg identificador do registrador
/// @param cep CEP do registrador
/// @param face face do registrador
/// @param numero número do registrador
/// @param x coordenada x do registrador
/// @param y coordenada y do registrador
/// @param verticeMaisProximo índice do vértice mais próximo
/// @return 1 se o registrador foi adicionado com sucesso, 0 caso contrário
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

/// @brief Busca um registrador nos registradores
/// @param regs ponteiro para os registradores
/// @param idReg identificador do registrador
/// @return índice do registrador se encontrado, -1 caso contrário
int buscaRegistrador(
    Registradores regs,
    const char *idReg
);

/// @brief Retorna o índice do vértice mais próximo de um registrador
/// @param regs ponteiro para os registradores
/// @param idReg identificador do registrador
/// @return índice do vértice mais próximo se o registrador for encontrado, -1 caso contrário
int getVerticeRegistrador(
    Registradores regs,
    const char *idReg
);

/// @brief Retorna a coordenada x de um registrador
/// @param regs ponteiro para os registradores
/// @param idReg identificador do registrador
/// @return coordenada x se o registrador for encontrado, 0 caso contrário
double getXRegistrador(
    Registradores regs,
    const char *idReg
);

/// @brief Retorna a coordenada y de um registrador
/// @param regs ponteiro para os registradores
/// @param idReg identificador do registrador
/// @return coordenada y se o registrador for encontrado, 0 caso contrário
double getYRegistrador(
    Registradores regs,
    const char *idReg
);

/// @brief Retorna o CEP de um registrador
/// @param regs ponteiro para os registradores
/// @param idReg identificador do registrador
/// @return ponteiro para o CEP se o registrador for encontrado, NULL caso contrário
char *getCepRegistrador(
    Registradores regs,
    const char *idReg
);

/// @brief Retorna a face de um registrador
/// @param regs ponteiro para os registradores
/// @param idReg identificador do registrador
/// @return face se o registrador for encontrado, 0 caso contrário
char getFaceRegistrador(
    Registradores regs,
    const char *idReg
);

/// @brief Retorna o número de um registrador
/// @param regs ponteiro para os registradores
/// @param idReg identificador do registrador
/// @return número se o registrador for encontrado, 0 caso contrário
double getNumeroRegistrador(
    Registradores regs,
    const char *idReg
);

/// @brief Imprime os registradores
/// @param regs ponteiro para os registradores
void printRegistradores(Registradores regs);

#endif