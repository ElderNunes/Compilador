# Compilador e opções comuns a todos os arquivos C++.
CXX := g++
CPPFLAGS := -Iinclude
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic

# Arquivos-fonte, objetos e dependências geradas automaticamente.
TARGET := lexer
BUILD_DIR := build
SOURCES := src/main.cpp src/lexer.cpp src/token.cpp
OBJECTS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
DEPENDENCIES := $(OBJECTS:.o=.d)

# Entrada padrão do alvo run e classificação da suíte fornecida.
ARQUIVO ?= tests/teste_leitura.txt
TEST_DIR := tests/textes_lexico/Testes
TESTES_VALIDOS := $(addprefix $(TEST_DIR)/programa,$(addsuffix .gyh,1 3 6 8 9 10 11))
TESTES_INVALIDOS := $(addprefix $(TEST_DIR)/programa,$(addsuffix .gyh,0 2 4 5 7 12))

.PHONY: all run test clean

all: $(TARGET)

# Liga os objetos e cria o executável final.
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

# Compila cada fonte separadamente dentro de build/.
$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

run: $(TARGET)
	./$(TARGET) "$(ARQUIVO)"

# Confere programas que devem ser aceitos e rejeitados.
test: $(TARGET) $(TESTES_VALIDOS) $(TESTES_INVALIDOS)
	@status=0; \
	for arquivo in $(TESTES_VALIDOS); do \
		if ./$(TARGET) "$$arquivo" >/dev/null 2>&1; then \
			printf '[OK] %s foi aceito\n' "$$arquivo"; \
		else \
			printf '[FALHOU] %s deveria ser aceito\n' "$$arquivo"; \
			status=1; \
		fi; \
	done; \
	for arquivo in $(TESTES_INVALIDOS); do \
		if ./$(TARGET) "$$arquivo" >/dev/null 2>&1; then \
			printf '[FALHOU] %s deveria ser rejeitado\n' "$$arquivo"; \
			status=1; \
		else \
			printf '[OK] %s foi rejeitado\n' "$$arquivo"; \
		fi; \
	done; \
	exit $$status

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Recompila os objetos necessários quando um header é alterado.
-include $(DEPENDENCIES)
