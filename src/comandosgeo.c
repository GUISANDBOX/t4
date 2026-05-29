#include <string.h>
#include "comandosgeo.h"
#include "hashfile.h"
#include "quadra.h"
#include "criasvg.h"

HashFile processaGeo(FILE *arqgeo, HashFile H, FILE *arqsvg) {
    char comando[100];
    int z, i;
    double x, y, h, w;
    char cep[10], sw[100], cfill[100], cstrk[100];

    fprintf(arqsvg, "<svg width=\"5000\" height=\"5000\" xmlns=\"http://www.w3.org/2000/svg\">\n");

    do {
        z = fscanf(arqgeo, " %99s", comando);
        if (z != 1) break;

        if (comando[0] == 'q') {
            fscanf(arqgeo, "%s %lf %lf %lf %lf", cep, &x, &y, &w, &h);
            Quadra q = criaQuadra(cep, x, y, w, h, sw, cfill, cstrk);
            if(!adicionarHashItem(&H, q, getCep(q))) {
                //printf("Erro ao adicionar item ao hashfile\n");
            }
            printf("Quadra criada: %s %lf %lf\n", cep, x, y);
            desenhaSVGQuadra(q, arqsvg);
        }
        else if (comando[0] == 'c') {
            fscanf(arqgeo, "%s %s %s", sw, cfill, cstrk);
        }
    } while (1);

    fprintf(arqsvg, "</svg>\n");
    return H;
}