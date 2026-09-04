# ============================================================
# Makefile - Simulador MLFQ (C++)
#
# Uso:
#   make            -> compila el ejecutable principal (bin/mlfq_simulator)
#   make run        -> compila y lo ejecuta
#   make test       -> compila y corre cada archivo de tests/
#   make clean      -> borra binarios y objetos compilados
#
# IMPORTANTE: correr estos comandos desde la terminal "MSYS2 MINGW64"
# (no PowerShell/cmd), porque las reglas usan utilidades tipo Unix
# (mkdir -p, rm -rf) que sí vienen incluidas ahí.
# ============================================================

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g -Iinclude

SRC_DIR   := src
BUILD_DIR := build
BIN_DIR   := bin
TEST_DIR  := tests

# Subcarpetas de código fuente por capa (agrega aquí si creas otra carpeta).
SRC_SUBDIRS := domain application infrastructure

# Todo el código "core" (todo excepto main.cpp): esto es lo que se
# comparte entre el ejecutable principal y los ejecutables de prueba.
CORE_SRCS := $(wildcard $(addsuffix /*.cpp,$(addprefix $(SRC_DIR)/,$(SRC_SUBDIRS))))
CORE_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CORE_SRCS))

MAIN_SRC := $(SRC_DIR)/main.cpp
MAIN_OBJ := $(BUILD_DIR)/main.o

TARGET := $(BIN_DIR)/mlfq_simulator

# Cada archivo tests/test_X.cpp se convierte en su propio ejecutable
# bin/test_X, enlazado contra todo el código core (pero sin main.cpp).
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_BINS := $(patsubst $(TEST_DIR)/%.cpp,$(BIN_DIR)/%,$(TEST_SRCS))

.PHONY: all run test clean

all: $(TARGET)

run: all
	./$(TARGET)

$(TARGET): $(CORE_OBJS) $(MAIN_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Regla genérica: compila cualquier src/<subcarpeta>/archivo.cpp
# a build/<subcarpeta>/archivo.o, creando la subcarpeta si no existe.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cada test se enlaza directo contra los .cpp core (no contra los .o,
# para no arrastrar problemas de orden de compilación mientras el
# proyecto es chico; si crece, se puede optimizar a $(CORE_OBJS)).
$(BIN_DIR)/%: $(TEST_DIR)/%.cpp $(CORE_SRCS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< $(CORE_SRCS) -o $@

test: $(TEST_BINS)
	@for t in $(TEST_BINS); do \
		echo "== Ejecutando $$t =="; \
		./$$t || exit 1; \
	done
	@echo "Todos los tests pasaron."

$(BUILD_DIR) $(BIN_DIR):
	@mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)