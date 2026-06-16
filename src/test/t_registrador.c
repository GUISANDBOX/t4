#include "unity.h"
#include <stdio.h>
#include "registrador.h"

void setUp(void) {}

void tearDown(void) {}

void test_adiciona_registrador(void) {
    Registradores regs = criaRegistradores(10);
    TEST_ASSERT_NOT_NULL(regs);

    int res1 = adicionaRegistrador(regs, "R0", "12345-678", 'N', 10.0, 50.0, 60.0, 1);
    int res2 = adicionaRegistrador(regs, "R1", "87654-321", 'S', 20.0, 30.0, 40.0, 2);
    int res3 = adicionaRegistrador(regs, "R2", "54321-987", 'L', 30.0, 70.0, 80.0, 3);
    int res4 = adicionaRegistrador(regs, "R4", "11111-111", 'O', 40.0, 90.0, 100.0, 4); 

    TEST_ASSERT_EQUAL_INT(1, res1);
    TEST_ASSERT_EQUAL_INT(1, res2);
    TEST_ASSERT_EQUAL_INT(1, res3);
    TEST_ASSERT_EQUAL_INT(1, res4); 
}


void test_busca_registrador(void) {
    Registradores regs = criaRegistradores(10);
    TEST_ASSERT_NOT_NULL(regs);

    adicionaRegistrador(regs, "R0", "12345-678", 'N', 10.0, 50.0, 60.0, 1);
    adicionaRegistrador(regs, "R1", "87654-321", 'S', 20.0, 30.0, 40.0, 2);
    adicionaRegistrador(regs, "R2", "54321-987", 'L', 30.0, 70.0, 80.0, 3);

    int res1 = getVerticeRegistrador(regs, "R0");
    int res2 = getVerticeRegistrador(regs, "R1");
    int res3 = getVerticeRegistrador(regs, "R2");

    TEST_ASSERT_EQUAL_INT(1, res1);
    TEST_ASSERT_EQUAL_INT(2, res2);
    TEST_ASSERT_EQUAL_INT(3, res3);

    float xR0 = getXRegistrador(regs, "R0");
    float yR0 = getYRegistrador(regs, "R0");
    char faceR0 = getFaceRegistrador(regs, "R0");
    double numeroR0 = getNumeroRegistrador(regs, "R0");
    char *cepR0 = getCepRegistrador(regs, "R0");
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 50.0, xR0);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 60.0, yR0);
    TEST_ASSERT_EQUAL_CHAR('N', faceR0);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 10.0, numeroR0);

    float xR1 = getXRegistrador(regs, "R1");
    float yR1 = getYRegistrador(regs, "R1");
    char faceR1 = getFaceRegistrador(regs, "R1");
    double numeroR1 = getNumeroRegistrador(regs, "R1");
    char *cepR1 = getCepRegistrador(regs, "R1");
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 30.0, xR1);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 40.0, yR1);
    TEST_ASSERT_EQUAL_CHAR('S', faceR1);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 20.0, numeroR1);

    float xR2 = getXRegistrador(regs, "R2");
    float yR2 = getYRegistrador(regs, "R2");
    char faceR2 = getFaceRegistrador(regs, "R2");
    double numeroR2 = getNumeroRegistrador(regs, "R2");
    char *cepR2 = getCepRegistrador(regs, "R2");
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 70.0, xR2);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 80.0, yR2);
    TEST_ASSERT_EQUAL_CHAR('L', faceR2);
    TEST_ASSERT_FLOAT_WITHIN(1e-12, 30.0, numeroR2);
}

void test_sobrescrita_registrador(void) {
    Registradores regs = criaRegistradores(10);
    TEST_ASSERT_NOT_NULL(regs);

    int res1 = adicionaRegistrador(regs, "R0", "12345-678", 'N', 10.0, 50.0, 60.0, 1);
    int res2 = adicionaRegistrador(regs, "R1", "87654-321", 'S', 20.0, 30.0, 40.0, 2);
    int res3 = adicionaRegistrador(regs, "R2", "54321-987", 'L', 30.0, 70.0, 80.0, 3);
    TEST_ASSERT_EQUAL_INT(1, res1);
    TEST_ASSERT_EQUAL_INT(1, res2);
    TEST_ASSERT_EQUAL_INT(1, res3);

    // Antes
    int verticeR0 = getVerticeRegistrador(regs, "R0");
    TEST_ASSERT_EQUAL_INT(1, verticeR0); // O vértice mais próximo de R0 deve ser 1 
    char *cepR0 = getCepRegistrador(regs, "R0");
    TEST_ASSERT_EQUAL_STRING("12345-678", cepR0); // O CEP de R0 deve ser "12345-678"

    int verticeR1 = getVerticeRegistrador(regs, "R1");
    TEST_ASSERT_EQUAL_INT(2, verticeR1); // O vértice mais
    char *cepR1 = getCepRegistrador(regs, "R1");
    TEST_ASSERT_EQUAL_STRING("87654-321", cepR1); // O CEP de
  
    int verticeR2 = getVerticeRegistrador(regs, "R2");
    TEST_ASSERT_EQUAL_INT(3, verticeR2); // O vértice mais
    char *cepR2 = getCepRegistrador(regs, "R2");
    TEST_ASSERT_EQUAL_STRING("54321-987", cepR2); // O CEP de

    // Tentar adicionar um registrador com o mesmo ID (deve sobrescrever os valores do mesmo registrador)
    res2 = adicionaRegistrador(regs, "R0", "99999-999", 'S', 20.0, 30.0, 40.0, 4);
    TEST_ASSERT_EQUAL_INT(1, res2); 
    verticeR0 = getVerticeRegistrador(regs, "R0");
    TEST_ASSERT_EQUAL_INT(4, verticeR0); // O vértice mais próximo de R0 deve ser atualizado para 4
    cepR0 = getCepRegistrador(regs, "R0");
    TEST_ASSERT_EQUAL_STRING("99999-999", cepR0); // O CEP de R0 deve ser atualizado para "99999-999"

    res3 = adicionaRegistrador(regs, "R2", "88888-888", 'O', 20.0, 30.0, 40.0, 2);
    TEST_ASSERT_EQUAL_INT(1, res3); 
    verticeR2 = getVerticeRegistrador(regs, "R2");
    TEST_ASSERT_EQUAL_INT(2, verticeR2); // O vértice mais
    cepR2 = getCepRegistrador(regs, "R2");
    TEST_ASSERT_EQUAL_STRING("88888-888", cepR2); // O CEP de R2 deve ser atualizado para "88888-888"


}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_adiciona_registrador);
  RUN_TEST(test_busca_registrador);
  RUN_TEST(test_sobrescrita_registrador);
  return UNITY_END();
}