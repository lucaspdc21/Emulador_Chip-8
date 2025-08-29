# Nome do executável final
TARGET = main

# Compilador e flags
CXX = g++

# -Wall: Habilita a maioria dos avisos de compilação
# -Wextra: Avisos adicionais ao wall
# -O2: Otimiza o código para melhor desempenho
CXXFLAGS = -Wall -Wextra -std=c++17 -O2

# Diretórios
SRC_DIR = .
OBJ_DIR = build
BIN_DIR = bin

# Todos os arquivos .cpp na origem
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
# Gera o nome dos arquivos .o de cada arquivo .cpp
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

all: $(BIN_DIR)/$(TARGET)

# Regra para criar o executável
$(BIN_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJS) -o $@

# Compila os arquivos .cpp em .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: all
	./$(BIN_DIR)/$(TARGET)

.PHONY: all clean run
