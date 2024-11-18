# Nome do executável
EXEC = floydWarshall

# Compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -Wall -std=c++17

# Pasta com o código-fonte
SRC = .

# Pasta de testes
TEST_DIR = Testes
CASE_DIR = $(TEST_DIR)/Casos-Teste
ANSWER_DIR = $(TEST_DIR)/Gabarito

# Objetivo principal
all: $(EXEC)

# Compilar o executável
$(EXEC): $(SRC)/floydWarshall.cpp
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(SRC)/floydWarshall.cpp

# Executar todos os casos de teste
test: $(EXEC)
	@echo "Executando testes..."
	@mkdir -p $(TEST_DIR)/Resultados
	@for i in $$(seq 1 20); do \
		echo "Testando caso $$i..."; \
		./$(EXEC) < $(CASE_DIR)/caso_floyd_warshall$$i.txt > $(TEST_DIR)/Resultados/output$$i.txt; \
		if diff -q $(TEST_DIR)/Resultados/output$$i.txt $(ANSWER_DIR)/output$$i.txt > /dev/null; then \
			echo "Caso $$i: Sucesso"; \
		else \
			echo "Caso $$i: Falha"; \
		fi \
	done

# Limpar arquivos gerados
clean:
	@rm -f $(EXEC)
	@rm -rf $(TEST_DIR)/Resultados
	@echo "Limpeza concluída."

