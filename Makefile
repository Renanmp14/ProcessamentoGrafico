CXX = clang++
INC = -Iinclude -I/opt/homebrew/include
LIBS = -L/opt/homebrew/lib -lglfw -framework OpenGL
COMM = Commun/glad.c

# Lista de arquivos com caminho completo
SRC = \
    src/TrabalhosGA/Atividade01/PoligonoPreenchido.cpp \
    src/TrabalhosGA/Atividade01/ApenasComContorno.cpp \
    src/TrabalhosGA/Atividade01/ApenasComPontos.cpp \
    src/TrabalhosGA/Atividade01/TresFormas.cpp \
    src/TrabalhosGA/Atividade01/Octagono.cpp \
    src/TrabalhosGA/Atividade01/Pentagono.cpp \
    src/TrabalhosGA/Atividade01/PacMan.cpp \
    src/TrabalhosGA/Atividade01/FatiaPizza.cpp \
    src/TrabalhosGA/Atividade01/Estrela.cpp \
    src/TrabalhosGA/Atividade01/Espiral.cpp \
    src/TrabalhosGA/Atividade01/DesenhoCuston.cpp \
    src/TrabalhosGA/Atividade02/Ex01.cpp \
    src/TrabalhosGA/Atividade02/Ex02.cpp \
    src/TrabalhosGA/Atividade02/Ex03.cpp \
    src/TrabalhosGA/Atividade02/ViewportComQuadrante.cpp \
    src/TrabalhosGA/Atividade02/ViewportCom4Quadrante.cpp \
    src/TrabalhosGA/Atividade02/TrianguloComClique.cpp \
    src/TrabalhosGB/Parte1/Exec1.cpp \
    src/TrabalhosGB/Parte1/Exec2.cpp \
    src/TrabalhosGB/Parte2/Exec3.cpp

# Extrai só o nome do executável de cada arquivo
TARGETS := $(notdir $(SRC))
TARGETS := $(patsubst %.cpp,%,$(TARGETS))

# make all compila todos os executáveis
all: $(TARGETS)

# Regra genérica para compilar cada arquivo
$(TARGETS):
	$(CXX) $(COMM) $(filter %/$@.cpp,$(SRC)) $(INC) $(LIBS) -o $@

FILE_SRC := $(filter %/$(FILE).cpp,$(SRC))

run: $(FILE_SRC)
	$(CXX) $(COMM) $(FILE_SRC) $(INC) $(LIBS) -o $(FILE)
	./$(FILE)
	
# Limpa todos os executáveis
clean:
	rm -f $(TARGETS)
