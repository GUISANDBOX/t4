#include "unity.h"
#include <stdio.h>
#include "grafo.h"

void setUp(void) {}

void tearDown(void) {
  // destruirHashFile(hash);
}

void test_cria_grafo(void) {
  Grafo g = criaGrafo(10);
  TEST_ASSERT_NOT_NULL(g);
  liberaGrafo(g);
}

void test_adiciona_vertice(void) {
  Grafo g = criaGrafo(10);
  TEST_ASSERT_NOT_NULL(g);

  int res1 = adicionaVertice(g, "A", 10.0, 20.0);
  int res2 = adicionaVertice(g, "B", 30.0, 40.0);
  int res3 = adicionaVertice(g, "C", 50.0, 60.0);
  int res4 = adicionaVertice(g, "A", 70.0, 80.0); // ID repetido

  TEST_ASSERT_EQUAL_INT(1, res1);
  TEST_ASSERT_EQUAL_INT(1, res2);
  TEST_ASSERT_EQUAL_INT(1, res3);
  TEST_ASSERT_EQUAL_INT(0, res4); // Deve falhar por ID repetido
  liberaGrafo(g);
}

void test_adiciona_aresta(void) {
  Grafo g = criaGrafo(10);
  TEST_ASSERT_NOT_NULL(g);

  adicionaVertice(g, "A", 10.0, 20.0);
  adicionaVertice(g, "B", 30.0, 40.0);

  int res1 = adicionaAresta(g, "A", "B", "12345-678", "87654-321", 100.0, 60.0, "Rua A-B");
  int res2 = adicionaAresta(g, "A", "C", "12345-678", "87654-321", 100.0, 60.0, "Rua A-C"); // Vertice C não existe

  TEST_ASSERT_EQUAL_INT(1, res1);
  TEST_ASSERT_EQUAL_INT(0, res2); // Deve falhar por vertice destino não existir

  liberaGrafo(g);
}

void test_imprime_grafo(void) {
  Grafo g = criaGrafo(10);
  TEST_ASSERT_NOT_NULL(g);

  adicionaVertice(g, "A", 10.0, 20.0);
  adicionaVertice(g, "B", 30.0, 40.0);
  adicionaVertice(g, "C", 50.0, 60.0);
  adicionaVertice(g, "D", 70.0, 80.0);

    adicionaAresta(g, "A", "B", "12345-678", "87654-321", 100.0, 60.0, "Rua A-B");
    adicionaAresta(g, "A", "C", "12345-678", "87654-321", 100.0, 60.0, "Rua A-C");
    adicionaAresta(g, "B", "C", "12345-678", "87654-321", 150.0, 50.0, "Rua B-C");
    adicionaAresta(g, "C", "D", "12345-678", "87654-321", 200.0, 40.0, "Rua C-D");
    adicionaAresta(g, "D", "A", "12345-678", "87654-321", 200.0, 40.0, "Rua D-A");
    

  printf("Imprimindo grafo:\n");
  imprimeGrafo(g);

  TEST_ASSERT_NOT_NULL(g); // Apenas para garantir que o grafo ainda existe
  liberaGrafo(g);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_cria_grafo);
  RUN_TEST(test_adiciona_vertice);
  RUN_TEST(test_adiciona_aresta);
  RUN_TEST(test_imprime_grafo);
  return UNITY_END();
}