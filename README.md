# Processamento Gráfico

Este repositório contém exemplos e códigos utilizados na disciplina de **Processamento Gráfico: Fundamentos** do curso de Ciência da Computação da Unisinos.

---

## 📂 Estrutura do Repositório

```plaintext
📂 main/
├── 📂 include/                # Cabeçalhos e bibliotecas de terceiros
│   ├── 📂 glad/               # Cabeçalhos da GLAD (OpenGL Loader)
│   │   ├── glad.h
│   ├── 📂 KHR/                # Cabeçalhos da Khronos (GLAD)
│   │   ├── khrplatform.h
├── 📂 common/                 # Código reutilizável entre os projetos
│   ├── glad.c                 # Implementação da GLAD
├── 📂 src/                    # Código-fonte dos exemplos e atividades
│   ├── 📂 Atividades/         # Diretórios com atividades específicas
├── 📄 MakeFile                # Configuração para compilação (Mac/Linux)
├── 📄 README.md               # Este arquivo
├── 📄 ComoCompilar.md         # Tutorial de compilação (Mac)
└── 📄 ...
```

---

## ⚠️ Dependências e Configuração

O projeto depende de **GLFW** e **GLAD** para funcionar corretamente.
Abaixo estão os passos para cada sistema operacional:

### 💻 MacOS

1. Instale o **Homebrew** (caso ainda não tenha):

```sh
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

2. Instale o **GLFW**:

```sh
brew install glfw
```

3. **GLAD** não vem pronto pelo Homebrew. Você deve gerar os arquivos em:
   [GLAD Generator](https://glad.dav1d.de/)

* **API:** OpenGL
* **Version:** 3.3+ (ou compatível com seu hardware)
* **Profile:** Core
* **Language:** C/C++

4. Extraia os arquivos gerados e organize da seguinte forma:

```plaintext
include/glad/glad.h
include/glad/KHR/khrplatform.h
common/glad.c
```

> Sem esses arquivos, a compilação falhará.

---

### 🖥️ Windows

1. Baixe e instale o **MinGW** ou use **Visual Studio** com suporte a C++.

2. Baixe o **GLFW**:

* Site oficial: [https://www.glfw.org/download.html](https://www.glfw.org/download.html)
* Coloque os arquivos `.lib` e headers na pasta do projeto ou configure o caminho de inclusão/liberação no Visual Studio.

3. Baixe a **GLAD** via [GLAD Generator](https://glad.dav1d.de/), da mesma forma do Mac.

4. Organize os arquivos no projeto:

```plaintext
include/glad/glad.h
include/glad/KHR/khrplatform.h
common/glad.c
```

> Configure o Visual Studio para incluir `include/` e linkar a GLFW.

---

## 📝 Sugestão de Estrutura para Atividades

Organize cada atividade em subdiretórios dentro de `src`:

```plaintext
src/
├── Atividade01/
│   ├── Ex1.cpp
│   ├── Ex2.cpp
│   └── ListaAtividade01.md
├── Atividade02/
│   ├── Ex1.cpp
│   └── ListaAtividade02.md
...
```

> Cada diretório de atividade pode conter um arquivo `ListaAtividade.md` com descrições e orientações sobre os exercícios.