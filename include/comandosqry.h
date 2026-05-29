#ifndef QRY_H
#define QRY_H
#include "hashfile.h"
#include <stdio.h>

void processaQry(FILE *arqqry, HashFile Hgeo, FILE *arqtxt, FILE *arqsvg);
/// @brief Processa os comandos do arquivo .qry e gera o arquivo .svg
/// @param arqqry Arquivo .qry
/// @param Hgeo Hashfile das quadras
/// @param arqtxt Arquivo .txt
/// @param arqsvg Arquivo .svg

#endif