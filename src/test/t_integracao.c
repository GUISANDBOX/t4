#include "hashfile.h"
#include "quadra.h"
#include "grafo.h"
#include "registrador.h"
#include "unity.h"
#include <stdio.h>

HashFile hash;
Grafo grafo;
char *fileName = "../output/hashfile.hf";


void setUp(void) {
  hash = criarHashFile(fileName, tamanhoQuadra(), 2048);
  hash = lerHashFile(fileName);
  Quadra q1 = criaQuadra("123456", 10.0, 10.0, 20.0, 15.0, "sw", "fill", "stroke");
  Quadra q2 = criaQuadra("789100", 50.0, 10.0, 20.0, 15.0, "sw", "fill", "stroke");
  int res1 = adicionarHashItem(&hash, q1, getCep(q1));
  int res2 = adicionarHashItem(&hash, q2, getCep(q2));
  grafo = criaGrafo(6);
  adicionaVertice(grafo, "1", 5.0, 5.0); // 0 
  adicionaVertice(grafo, "2", 40.0, 5.0); // 1
  adicionaVertice(grafo, "3", 80.0, 5.0); // 2
  adicionaVertice(grafo, "4", 5.0, 30.0); // 3
  adicionaVertice(grafo, "5", 40.0, 30.0); // 4
  adicionaVertice(grafo, "6", 80.0, 30.0); // 5

  // Criando as arestas do grafo
  adicionaAresta(grafo, "1", "2", "123456", "-", 30.0, 60.0, "Rua 1-2");
  adicionaAresta(grafo, "1", "4", "-", "123456", 40.0, 50.0, "Rua 1-4");
  adicionaAresta(grafo, "4", "5", "-", "123456", 25.0, 70.0, "Rua 4-5");
  adicionaAresta(grafo, "2", "5", "123456", "789100", 25.0, 150.0, "Rua 2-5");
  adicionaAresta(grafo, "2", "3", "789100", "-", 25.0, 20.0, "Rua 2-3");
  adicionaAresta(grafo, "3", "6", "789100", "-", 30.0, 60.0, "Rua 3-6");
  adicionaAresta(grafo, "5", "6", "-", "789100", 40.0, 50.0, "Rua 5-6");
}

void tearDown(void) {
  liberaGrafo(grafo);
  destruirHashFile(hash);
  remove(fileName);
  remove("../output/hashfile.hfc");
}

void teste_insercao_registrador(void) {
  double px, py;
  Registradores regs = criaRegistradores(10);
  char face;
  double numero;

  // Busca a quadra no hashfile 
  Quadra q = (Quadra)buscarHashItem(hash, "123456");

  // Adiciona no registrador na face N, número 10.0, e calcula o ponto do endereço correspondente
  face = 'N';
  numero = 10.0;
  calculaPontoEndereco(q, face, numero, &px, &py);
  int vMaisProximo = verticeMaisProximo(grafo, px, py);
  adicionaRegistrador(regs, "R0", getCep(q), face, numero, px, py, vMaisProximo);
  TEST_ASSERT_FLOAT_WITHIN(1e-12, 20.0, px);
  TEST_ASSERT_FLOAT_WITHIN(1e-12, 25.0, py);
  int verticeR0 = getVerticeRegistrador(regs, "R0");
  TEST_ASSERT_EQUAL_INT(3, verticeR0); // O vértice mais próximo de R0 deve ser 3 
  char *cepR0 = getCepRegistrador(regs, "R0");
  TEST_ASSERT_EQUAL_STRING("123456", cepR0);
  char *idV0 = getIdVertice(grafo, verticeR0);
  TEST_ASSERT_EQUAL_STRING("4", idV0); // O vértice mais próximo de R0 deve ser o vértice "4"

  // Adiciona no registrador na face S, número 10.0, e calcula o ponto do endereço correspondente
  face = 'S';
  numero = 10.0;
  calculaPontoEndereco(q, face, numero, &px, &py);
  vMaisProximo = verticeMaisProximo(grafo, px, py);
  adicionaRegistrador(regs, "R1", getCep(q), face, numero, px, py, vMaisProximo);
  TEST_ASSERT_FLOAT_WITHIN(1e-12, 20.0, px);
  TEST_ASSERT_FLOAT_WITHIN(1e-12, 10.0, py);
  int verticeR1 = getVerticeRegistrador(regs, "R1");
  TEST_ASSERT_EQUAL_INT(0, verticeR1); // O vértice mais próximo de R1 deve ser 3
  char *cepR1 = getCepRegistrador(regs, "R1");
  TEST_ASSERT_EQUAL_STRING("123456", cepR1);
  char *idV1 = getIdVertice(grafo, verticeR1);
  TEST_ASSERT_EQUAL_STRING("1", idV1); // O vértice mais próximo

  // Adiciona no registrador na face L, número 10.0, e calcula o ponto do endereço correspondente
  face = 'L';
  numero = 10.0;
  calculaPontoEndereco(q, face, numero, &px, &py);
  vMaisProximo = verticeMaisProximo(grafo, px, py);
  adicionaRegistrador(regs, "R2", getCep(q), face, numero, px, py, vMaisProximo);
  TEST_ASSERT_FLOAT_WITHIN(1e-12, 10.0, px);
  TEST_ASSERT_FLOAT_WITHIN(1e-12, 20.0, py);
  int verticeR2 = getVerticeRegistrador(regs, "R2");
  TEST_ASSERT_EQUAL_INT(3, verticeR2); // O vértice mais próximo de R2 deve ser 3
  char *cepR2 = getCepRegistrador(regs, "R2");
  TEST_ASSERT_EQUAL_STRING("123456", cepR2);
  char *idV2 = getIdVertice(grafo, verticeR2);
  TEST_ASSERT_EQUAL_STRING("4", idV2); // O vértice mais próximo de R2 deve ser o vértice "4"

  // Adiciona no registrador na face O, número 10.0, e calcula o ponto do endereço correspondente
  face = 'O';
  numero = 10.0;
  calculaPontoEndereco(q, face, numero, &px, &py);
  vMaisProximo = verticeMaisProximo(grafo, px, py);
  adicionaRegistrador(regs, "R3", getCep(q), face, numero, px, py, vMaisProximo);
  TEST_ASSERT_FLOAT_WITHIN(1e-12, 30.0, px);
  TEST_ASSERT_FLOAT_WITHIN(1e-12, 20.0, py);
  int verticeR3 = getVerticeRegistrador(regs, "R3");
  TEST_ASSERT_EQUAL_INT(4, verticeR3); // O vértice mais próximo de R3 deve ser 5
  char *cepR3 = getCepRegistrador(regs, "R3");
  TEST_ASSERT_EQUAL_STRING("123456", cepR3);
  char *idV3 = getIdVertice(grafo, verticeR3);
  TEST_ASSERT_EQUAL_STRING("5", idV3); // O vértice mais próximo de R3 deve ser o vértice "5"


  imprimeGrafo(grafo);
}


void test_busca_grafo(void) {
  double px, py;
  Registradores regs = criaRegistradores(10);
  char face;
  double numero;

  // Busca a quadra no hashfile 
  Quadra q1 = (Quadra)buscarHashItem(hash, "123456");
  Quadra q2 = (Quadra)buscarHashItem(hash, "789100");

  // Adiciona no registrador na face N, número 10.0, e calcula o ponto do endereço correspondente
  face = 'S';
  numero = 10.0;
  calculaPontoEndereco(q1, face, numero, &px, &py);
  int vMaisProximo = verticeMaisProximo(grafo, px, py);
  adicionaRegistrador(regs, "R1", getCep(q1), face, numero, px, py, vMaisProximo);
  int verticeR1 = getVerticeRegistrador(regs, "R1");

  face = 'O';
  numero = 15.0;
  calculaPontoEndereco(q2, face, numero, &px, &py);
  vMaisProximo = verticeMaisProximo(grafo, px, py);
  adicionaRegistrador(regs, "R2", getCep(q2), face, numero, px, py, vMaisProximo);
  int verticeR2 = getVerticeRegistrador(regs, "R2");

  // Teste do algoritmo de Dijkstra para encontrar o caminho mais curto entre os registradores R1 e R2
  ResultadoDijkstra res = dijkstra(grafo, verticeR1, verticeR2, MENOR_DISTANCIA);
  TEST_ASSERT_TRUE(existeCaminho(res)); // Deve existir um caminho entre R1 e R2
  double custo = getCustoResultado(res);
  TEST_ASSERT_FLOAT_WITHIN(1e-12, 85.00, custo); // O custo do caminho mais rápido deve ser 85.00
  imprimeCaminho(grafo, res, NULL);

  res = dijkstra(grafo, verticeR1, verticeR2, MENOR_TEMPO);
  TEST_ASSERT_TRUE(existeCaminho(res)); // Deve existir um caminho entre R1 e R2
  custo = getCustoResultado(res);
  TEST_ASSERT_FLOAT_WITHIN(0.001, 1.4667, custo); // O custo do caminho mais rápido deve ser 1.4667
  imprimeCaminho(grafo, res, NULL);

  liberaResultadoDijkstra(res);

  // Teste do algoritmo de Dijkstra para encontrar o caminho mais curto entre os registradores R2 a R1
  res = dijkstra(grafo, verticeR2, verticeR1, MENOR_DISTANCIA);
  TEST_ASSERT_FALSE(existeCaminho(res)); // Não deve existir um caminho entre R2 e R1
  custo = getCustoResultado(res);
  TEST_ASSERT_FLOAT_WITHIN(1e-12, INF, custo); // O custo do caminho mais rápido deve ser 0.0
  imprimeCaminho(grafo, res, NULL);

  liberaResultadoDijkstra(res);
}


void test_altera_velocidade(void) {

  imprimeComponentesFortesVelocidade(grafo, 50.0, NULL); // Antes de alterar a velocidade, imprime os componentes fortemente conexos com limite de velocidade 100.0

  // Altera a velocidade das arestas dentro da região (0, 0) a (100, 100) para 100.0
  alteraVelocidade(grafo, 100.0, 0.0, 0.0, 50.0, 50.0);

  // Testa se a velocidade das arestas foi alterada corretamente
  imprimeGrafo(grafo);
}


// simple test runner
int main(void) {
  UNITY_BEGIN();
  RUN_TEST(teste_insercao_registrador);
  RUN_TEST(test_busca_grafo);
  RUN_TEST(test_altera_velocidade);
  return UNITY_END();
}