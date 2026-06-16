#include "hashfile.h"
#include "quadra.h"
#include "grafo.h"
#include "registrador.h"
#include "unity.h"
#include <stdio.h>

HashFile hash;
Grafo grafo;

void setUp(void) {
  hash = criarHashFile("../output/hashfile.hf", tamanhoQuadra(), 2048);
  hash = lerHashFile("../output/hashfile.hf");
  Quadra q1 = criaQuadra("123456", 10.0, 10.0, 20.0, 15.0, "sw", "fill", "stroke");
  Quadra q2 = criaQuadra("789100", 50.0, 10.0, 20.0, 15.0, "sw", "fill", "stroke");
  int res1 = adicionarHashItem(&hash, q1, getCep(q1));
  int res2 = adicionarHashItem(&hash, q2, getCep(q2));
  grafo = criaGrafo(6);
  adicionaVertice(grafo, "1", 5.0, 5.0);
  adicionaVertice(grafo, "2", 40.0, 5.0);
  adicionaVertice(grafo, "3", 80.0, 5.0);
  adicionaVertice(grafo, "4", 5.0, 30.0);
  adicionaVertice(grafo, "5", 40.0, 30.0);
  adicionaVertice(grafo, "6", 80.0, 30.0);

}

void tearDown(void) {
  // destruirHashFile(hash);
}

void test_registrador(void) {
  double px, py;
  char face = 'N';
  double num = 10.0;
  Registradores regs = criaRegistradores(10);
  Quadra q = (Quadra)buscarHashItem(hash, "123456");
  calculaPontoEndereco(q, face, num, &px, &py);
  int vMaisProximo = verticeMaisProximo(grafo, px, py);
  adicionaRegistrador(regs, "R0", getCep(q), face, num, px, py, vMaisProximo);
  int verticeR0 = getVerticeRegistrador(regs, "R0");
  TEST_ASSERT_EQUAL_INT(0, verticeR0); // O vértice mais próximo de R0 deve ser 1 
  char *cepR0 = getCepRegistrador(regs, "R0");
  TEST_ASSERT_EQUAL_STRING("123456", cepR0);
  printf("Vertice mais próximo: %d => %s\n", vMaisProximo, getIdVertice(grafo, vMaisProximo));
}


// simple test runner
int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_registrador);
  return UNITY_END();
}