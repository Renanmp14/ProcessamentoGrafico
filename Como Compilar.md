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

# Variável para escolher qual arquivo compilar
FILE ?= PoligonoPreenchido
ACTIVITY ?= Atividade01

SRC_DIR = src/TrabalhosGA/$(ACTIVITY)
SRC = $(SRC_DIR)/$(FILE).cpp $(COMM)
TARGET = $(FILE)

all:
	$(CXX) $(SRC) $(INC) $(LIBS) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
```

---

## 4. Como compilar e rodar

### 🔹 Compilar um arquivo da Atividade 01

```sh
make ACTIVITY=Atividade01 FILE=PoligonoPreenchido
```

### 🔹 Rodar diretamente após compilar

```sh
make run ACTIVITY=Atividade01 FILE=PoligonoPreenchido
```

### 🔹 Compilar outro arquivo (por exemplo, da Atividade 02)

```sh
make ACTIVITY=Atividade02 FILE=OutroExemplo
```

### 🔹 Limpar os binários gerados

```sh
make clean ACTIVITY=Atividade01 FILE=PoligonoPreenchido
```
