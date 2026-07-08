#ifndef ITEM_H
#define ITEM_H

// Programa de TAD para representar um item com chave e valor

/// @brief Possui chave e valor - definir como uma struct.
typedef void* Item; 

/// @brief Cria um item com chave e valor
/// @param chave chave do item
/// @param valor valor do item
/// @return item criado
Item criarItem(int chave, void* valor);

/// @brief Imprime o item no formato [chave] valor
/// @param item 
void printItem(Item item);

/// @brief Retorna a chave do item
/// @param item Objeto do tipo Item
/// @return chave do item
int getChave(Item item);

/// @brief Retorna o valor do item
/// @param item Objeto do tipo Item
/// @return valor do item
void* getValor(Item item);

#endif