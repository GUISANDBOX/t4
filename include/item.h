#ifndef ITEM_H
#define ITEM_H

/// @brief Possui chave e valor - definir como uma struct.
typedef void* Item; 

Item criarItem(int chave, void* valor);

void printItem(Item item);

int getChave(Item item);

void* getValor(Item item);

#endif