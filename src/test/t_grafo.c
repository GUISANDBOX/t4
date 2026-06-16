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

  int quantidadeVertices = getQuantidadeVertices(g);
  TEST_ASSERT_EQUAL_INT(4, quantidadeVertices); // Deve ter 4 vértices

  int quantidadeArestas = getQuantidadeArestas(g);
  TEST_ASSERT_EQUAL_INT(5, quantidadeArestas); // Deve ter 5 arestas

  int indiceA = buscaIndiceVertice(g, "A");
  TEST_ASSERT_EQUAL_INT(0, indiceA); // O vértice A deve estar
  TEST_ASSERT_EQUAL_STRING("A", getIdVertice(g, indiceA)); // O vértice A deve ter ID "A"
  TEST_ASSERT_FLOAT_WITHIN(1e-12, 10.0, getXVertice(g, indiceA)); // O vértice A deve ter coordenada x = 10.0
  TEST_ASSERT_FLOAT_WITHIN(1e-12, 20.0, getYVertice(g, indiceA)); // O vértice A deve ter coordenada y = 20.0

  int indiceB = buscaIndiceVertice(g, "B");
  TEST_ASSERT_EQUAL_INT(1, indiceB); // O vértice B deve estar
  TEST_ASSERT_EQUAL_STRING("B", getIdVertice(g, indiceB)); // O vértice B deve ter ID "B"
  TEST_ASSERT_FLOAT_WITHIN(1e-12, 30.0, getXVertice(g, indiceB)); // O vértice B deve ter coordenada x = 30.0
  TEST_ASSERT_FLOAT_WITHIN(1e-12, 40.0, getYVertice(g, indiceB)); // O vértice B deve ter coordenada y = 40.0

  int indiceC = buscaIndiceVertice(g, "C");
  TEST_ASSERT_EQUAL_INT(2, indiceC); // O vértice C deve estar
  TEST_ASSERT_EQUAL_STRING("C", getIdVertice(g, indiceC)); // O vértice C deve ter ID "C"
  TEST_ASSERT_FLOAT_WITHIN(1e-12, 50.0, getXVertice(g, indiceC)); // O vértice C deve ter coordenada x = 50.0
  TEST_ASSERT_FLOAT_WITHIN(1e-12, 60.0, getYVertice(g, indiceC)); // O vértice C deve ter coordenada y = 60.0

  int indiceD = buscaIndiceVertice(g, "D");
  TEST_ASSERT_EQUAL_INT(3, indiceD); // O vértice D deve estar
  TEST_ASSERT_EQUAL_STRING("D", getIdVertice(g, indiceD)); // O vértice D deve ter ID "D"
  TEST_ASSERT_FLOAT_WITHIN(1e-12, 70.0, getXVertice(g, indiceD));  // O vértice D deve ter coordenada x = 70.0
  TEST_ASSERT_FLOAT_WITHIN(1e-12, 80.0, getYVertice(g, indiceD)); // O vértice D deve ter coordenada y = 80.0

  liberaGrafo(g);
}

void test_vertice_mais_proximo(void) {
  Grafo g = criaGrafo(10);
  TEST_ASSERT_NOT_NULL(g);

  adicionaVertice(g, "A", 10.0, 20.0);
  adicionaVertice(g, "B", 30.0, 40.0);
  adicionaVertice(g, "C", 50.0, 60.0);

  int indiceMaisProximo = verticeMaisProximo(g, 12.0, 22.0);
  char *idVertice = getIdVertice(g, indiceMaisProximo);
  TEST_ASSERT_EQUAL_STRING("A", idVertice); // O vértice A é o mais próximo de (12, 22)

  indiceMaisProximo = verticeMaisProximo(g, 35.0, 45.0);
  idVertice = getIdVertice(g, indiceMaisProximo);
  TEST_ASSERT_EQUAL_STRING("B", idVertice); // O vértice B é o mais próximo de (35, 45)

  indiceMaisProximo = verticeMaisProximo(g, 55.0, 65.0);
  idVertice = getIdVertice(g, indiceMaisProximo);
  TEST_ASSERT_EQUAL_STRING("C", idVertice); // O vértice C é o mais próximo de (55, 65)

  indiceMaisProximo = verticeMaisProximo(g, 10.0, 20.0);
  idVertice = getIdVertice(g, indiceMaisProximo);
  TEST_ASSERT_EQUAL_STRING("A", idVertice); // O vértice A é o mais próximo de (10, 20)

  liberaGrafo(g);
}



int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_cria_grafo);
  RUN_TEST(test_adiciona_vertice);
  RUN_TEST(test_adiciona_aresta);
  RUN_TEST(test_imprime_grafo);
  RUN_TEST(test_vertice_mais_proximo);
  return UNITY_END();
}