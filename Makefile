CXX = clang++
INC = -Iinclude -I/opt/homebrew/include
LIBS = -L/opt/homebrew/lib -lglfw -framework OpenGL

COMM = Commun/glad.c

# Variável para escolher qual arquivo compilar
FILE ?= File
ACTIVITY ?= Folder

SRC_DIR = src/TrabalhosGA/$(ACTIVITY)
SRC = $(SRC_DIR)/$(FILE).cpp $(COMM)
TARGET = $(FILE)

all:
	$(CXX) $(SRC) $(INC) $(LIBS) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
