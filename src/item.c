#include <stdio.h>
#include <stdlib.h>
#include "item.h"

struct sItem {
    int chave;
    void* valor;
};

typedef struct sItem sItem;

Item criarItem(int chave, void* valor) {
    sItem* novo = (sItem*)malloc(sizeof(sItem));
    if (!novo) {
        printf("Erro na alocação de item\n");
        return NULL;
    }
    novo->chave = chave;
    novo->valor = valor;
    return (Item)novo;
}

void printItem(Item item) {
    sItem* i = (sItem*)item;
    printf("[%d] %s\n", i->chave, (char *)i->valor);
}

int getChave(Item item) {
    sItem* i = (sItem*)item;
    return i->chave;
}

void* getValor(Item item) {
    sItem* i = (sItem*)item;
    return i->valor;
}