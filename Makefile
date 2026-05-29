PROJ_NAME=ted
ALUNO=
LIBS=

CC = gcc
CFLAGS = -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration -Iinclude
LDFLAGS = -O0
SOURCES = main.c $(wildcard src/*.c)
OBJETOS = $(SOURCES:.c=.o)

# Unity testing framework settings
UNITY_DIR = unity/src
UNITY_INC = -I$(UNITY_DIR)
TEST_SOURCES = $(wildcard test/*.c)
TEST_TARGET = run_tests
TEST_TESTS = $(patsubst test/%.c,run_test_%,$(TEST_SOURCES))
TEST_APP_SOURCES = $(filter-out main.c,$(SOURCES))

all: $(PROJ_NAME)

$(PROJ_NAME): $(OBJETOS)
	$(CC) -o $(PROJ_NAME) $(LDFLAGS) $(OBJETOS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJETOS) $(PROJ_NAME) $(TEST_TESTS) *.hf *.hfc  output/*

run: $(PROJ_NAME)
	./$(PROJ_NAME)

# build and run unit tests
.PHONY: all clean run $(TEST_TARGET) $(TEST_TESTS)

$(TEST_TARGET): $(TEST_TESTS)

$(TEST_TESTS): run_test_%: test/%.c $(TEST_APP_SOURCES) $(UNITY_DIR)/unity.c
	$(CC) $(CFLAGS) $(UNITY_INC) -o $@ $(TEST_APP_SOURCES) $< $(UNITY_DIR)/unity.c

$(TEST_TARGET): $(TEST_TESTS)
	@for t in $(TEST_TESTS); do echo "Running $$t"; ./$$t; done