#ifndef QRY_H
#define QRY_H
#include "hashfile.h"
#include "grafo.h"
#include <stdio.h>


void processaQry(FILE *arqqry, HashFile Hgeo, Grafo grafo, FILE *arqtxt, FILE *arqsvg);
/// @brief Processa os comandos do arquivo .qry e gera o arquivo .svg
/// @param arqqry Arquivo .qry
/// @param Hgeo Hashfile das quadras
/// @param grafo Grafo representando o arquivo viário
/// @param arqtxt Arquivo .txt
/// @param arqsvg Arquivo .svg

#endif