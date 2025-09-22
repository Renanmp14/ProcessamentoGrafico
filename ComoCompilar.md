# 📘 Guia de Compilação e Execução no macOS

## 1. Dependências necessárias no macOS

Antes de compilar os projetos, é necessário instalar e configurar as bibliotecas usadas:

### 🔹 GLFW

No macOS, você pode instalar o **GLFW** usando o Homebrew:

```sh
brew install glfw
```

### 🔹 GLAD

O **GLAD** não vem pronto no Homebrew. Você precisa **gerar** os arquivos em:

👉 [https://glad.dav1d.de/](https://glad.dav1d.de/)

Configurações no site:

* **Language**: C/C++
* **Specification**: OpenGL
* **API gl**: 3.3 (ou superior, até 4.1 no macOS)
* **Profile**: Core
* **Options**: “Generate a loader” marcado ✅

Depois de baixar, você usará os arquivos:

* `glad.c`
* `glad.h`
* `khrplatform.h`

Esses arquivos devem ser colocados na estrutura do projeto conforme abaixo.

---

## 2. Estrutura de pastas

O projeto foi organizado da seguinte forma:

```
ProcessamentoGrafico/
│
├── Commun/
│   └── glad.c
│
├── include/
│   ├── glad/
│   │   └── glad.h
│   └── KHR/
│       └── khrplatform.h
│
├── src/
│   └── TrabalhosGA/
│       ├── Atividade01/
│       │   └── Códigos01.cpp
│       └── Atividade02/
│           └── Códigos02.cpp
│
└── Makefile
```

---

## 3. Makefile

O projeto utiliza um **Makefile genérico** para compilar diferentes atividades.

Exemplo de Makefile:

```makefile
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
    src/TrabalhosGA/Atividade02/TrianguloComClique.cpp

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

```

---

## 4. Como compilar e rodar

### 🔹 Compilar um arquivo (Apenas um Arquivo)

```sh
make PoligonoPreenchido
```

### 🔹 Compilar Todos os Arquivos

```sh
make all
```

### 🔹 Rodar diretamente após compilar (Apenas um Arquivo)

```sh
make run FILE=PoligonoPreenchido
```

### 🔹 Limpar os binários gerados (Apenas um Arquivo)

```sh
make clean FILE=PoligonoPreenchido
```

### 🔹 Limpar todos os Arquivos

```sh
make clean
```
