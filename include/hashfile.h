#ifndef HASHFILE
#define HASHFILE
#include <stdio.h>

// programa de hashfile, onde cada item é um ponteiro genérico (void*), e o
// hashfile em si também é um ponteiro genérico (void*).

typedef void *HashItem; // Define que cada item do hashfile é um item genérico
typedef void *HashFile; // Define o tipo HashFile

/// @brief Cria um hashfile
/// @param nome nome do arquivo
/// @param recordSize tamanho em bytes de cada registro armazenado
/// @param bucketSize tamanho em bytes de cada página de bucket
/// @return objeto HashFile
HashFile criarHashFile(char *nome, int recordSize, int bucketSize);

/// @brief lê um hashfile
/// @param file_name nome do arquivo
/// @return objeto HashFile
HashFile lerHashFile(char *file_name);

/// @brief retorna o número de buckets do hashfile
/// @param hash hashfile a ser consultado
/// @return número de buckets
int getBucketsLength(HashFile hash);

/// @brief Retorna a chave de bucket usando os bits menos significativos de
/// depth
/// @param key string da chave
/// @param depth número de bits do hash a serem usados
/// @return valor inteiro do bucket
int getKey(char *key, int depth);

/// @brief Adiciona um item ao hashfile
/// @param hash hashfile onde o item será adicionado
/// @param item item a ser adicionado
/// @return 1 se o item foi adicionado com sucesso, 0 caso contrário
int adicionarHashItem(HashFile *hash, HashItem item, char *key);

/// @brief Busca um item no hashfile pela chave
/// @param hash hashfile onde o item será buscado
/// @param key string da chave usada para buscar o item
/// @return o item encontrado ou NULL se não encontrado
HashItem buscarHashItem(HashFile hash, char *key);

/// @brief Atualiza um item existente no hashfile pela chave
/// @param hash hashfile onde o item será atualizado
/// @param item item atualizado
/// @param key chave usada para buscar o item
/// @return 1 se o item foi atualizado com sucesso, 0 caso contrário
int atualizarHashItem(HashFile *hash, HashItem item, char *key);

/// @brief Função hash simples para strings
/// @param str string a ser hasheada
/// @return valor hash
int hashString(char *str);

/// @brief Imprime informações do hashfile
/// @param hash hashfile a ser consultado
void printHashFileInfo(HashFile hash);

/// @brief Remove um item do hashfile pela chave
/// @param hash hashfile onde o item será removido
/// @param key chave usada para buscar o item
void removerHashItem(HashFile hash, char *key);

/// @brief Retorna uma lista de todos os itens do hashfile
/// @param hash hashfile a ser consultado
/// @param itens array de ponteiros para receber os itens
/// @return número de itens
int getListaItens(HashFile hash, HashItem *itens);

#endif