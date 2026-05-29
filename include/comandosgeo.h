#ifndef GEO_H
#define GEO_H
#include "hashfile.h"
#include <stdio.h>

/// @brief Processa os comandos do arquivo .geo e adiciona os elementos no hashfile e no arquivo svg
/// @param arqgeo arquivo .geo
/// @param H hashfile das quadras
/// @param arqsvg arquivo .svg
/// @return hashfile das quadras
HashFile processaGeo(FILE *arqgeo, HashFile H, FILE *arqsvg);

#endif