#include "hashfile.h"
#include "quadra.h"
#include "unity.h"
#include <stdio.h>

HashFile hash;

void setUp(void) {}

void tearDown(void) {
  // destruirHashFile(hash);
}

void test_tamanho_struct(void) {
  Quadra q = criaQuadra("123456780000", 10.0, 20.0, 30.0, 40.0, "sw", "fill",
                        "stroke");

  printf("Tamanho real da struct Quadra: %zu bytes\n", tamanhoQuadra());

  destruirQuadra(q);
}

void test_inicializacao_hash_d2(void) {
  printf("Iniciando teste de inicialização do hashfile...\n");
  hash = criarHashFile("output/hashfile.hf", tamanhoQuadra(), 2048);
  hash = lerHashFile("output/hashfile.hf");
  TEST_ASSERT_NOT_NULL(hash);
}

void test_inserir_item_hash(void) {
  hash = lerHashFile("output/hashfile.hf");
  Quadra q1 = criaQuadra("123456780000", 10.0, 20.0, 30.0, 40.0, "sw", "fill",
                         "stroke");
  Quadra q2 = criaQuadra("123456780001", 10.0, 20.0, 30.0, 40.0, "sw", "fill",
                         "stroke");
  Quadra q3 = criaQuadra("123456780002", 10.0, 20.0, 30.0, 40.0, "sw", "fill",
                         "stroke");
  Quadra q4 = criaQuadra("123456780004", 10.0, 20.0, 30.0, 40.0, "sw", "fill",
                         "stroke");
  int res1 = adicionarHashItem(&hash, q1, getCep(q1));
  int res2 = adicionarHashItem(&hash, q2, getCep(q2));
  int res3 = adicionarHashItem(&hash, q3, getCep(q3));
  int res4 = adicionarHashItem(&hash, q4, getCep(q4));
  printHashFileInfo(hash);
  Quadra busca = buscarHashItem(hash, getCep(q1));
  if (busca) {
    printf("Item encontrado: %s\n", getCep(busca));
    printQuadra(busca);
  } else {
    printf("Item não encontrado\n");
  }

  busca = buscarHashItem(hash, getCep(q2));
  if (busca) {
    printf("Item encontrado: %s\n", getCep(busca));
    printQuadra(busca);
  } else {
    printf("Item não encontrado\n");
  }
  TEST_ASSERT_NOT_NULL(hash);
  TEST_ASSERT_NOT_NULL(busca);
}

void test_buscar_quadra_existente_hash(void) {
  HashItem encontrado;
  printf("\nTESTE DE BUSCA NO HASHFILE QUADRA\n");
  hash = lerHashFile("hashquadra.hf");

  if (!hash) {
    printf("Erro ao ler hashfile para teste de busca\n");
    return;
  }

  encontrado = buscarHashItem(hash, "b01.1");
  if (encontrado)
    printQuadra((Quadra)encontrado);
  TEST_ASSERT_NOT_NULL(encontrado);

  encontrado = buscarHashItem(hash, "b03.10");
  if (encontrado)
    printQuadra((Quadra)encontrado);
  TEST_ASSERT_NOT_NULL(encontrado);

  encontrado = buscarHashItem(hash, "b010.10");
  if (encontrado)
    printQuadra((Quadra)encontrado);
  TEST_ASSERT_NOT_NULL(encontrado);
}

// simple test runner
int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_tamanho_struct);
  RUN_TEST(test_inicializacao_hash_d2);
  return UNITY_END();
}