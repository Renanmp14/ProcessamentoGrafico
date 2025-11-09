# 🧩 Atividades de Processamento Gráfico — OpenGL 3.3+

Este repositório contém as resoluções dos exercícios propostos na disciplina **Processamento Gráfico: Fundamentos**, ministrada por **Rossana Baptista Queiroz**, utilizando **OpenGL moderno (3.3+)** com **GLFW**, **GLAD** e **GLM**.

---

## 🧠 Objetivo geral

Praticar a **criação e manipulação de geometria básica** em OpenGL moderno, entendendo:

* Como criar e renderizar triângulos usando **VAO/VBO**;
* Como gerenciar **múltiplas instâncias** de objetos;
* Como aplicar **transformações com matrizes** (usando a biblioteca **GLM**);
* E como **interagir com o mouse** para criar objetos dinamicamente.

---

## 🧱 Estrutura do projeto

```
Commun/
 └── glad.c              # Implementação do GLAD
include/
 └── glad, GLFW, GLM...  # Cabeçalhos das bibliotecas
src/
 └── TrabalhosGB/
      └── Parte1/
           ├── Exec1.cpp  # Exercício 1 - Triângulo básico
           ├── Exec2.cpp  # Exercício 2 - 5 triângulos instanciados
           └── Exec3.cpp  # Exercício 3 - Triângulos com GLM e cliques
Makefile
README.md
```

---

## 🧩 **Parte 1 — Sem matriz de transformação**

### 🔹 Exercício 1 — Triângulo básico

**Arquivo:** `Exec1.cpp`

**Solicitação:**

> Criar uma função
> `GLuint createTriangle(float x0, float y0, float x1, float y1, float x2, float y2);`
> que gera um triângulo com as coordenadas passadas e retorna seu VAO.

**Descrição da solução:**
O programa inicializa o contexto OpenGL e renderiza **um triângulo vermelho** no centro da tela, usando shaders simples e buffer único.

---

### 🔹 Exercício 2 — Instanciando 5 triângulos

**Arquivo:** `Exec2.cpp`

**Solicitação:**

> Instanciar 5 triângulos na tela utilizando a função `createTriangle`.
> Armazenar os VAOs em um array ou `std::vector` e desenhá-los no loop principal.

**Descrição da solução:**

* O programa cria **5 triângulos** alinhados horizontalmente na parte inferior da tela.
* Todos usam o mesmo shader, mas cada um tem um VAO próprio.
* A função `renderTriangles()` percorre o vetor e desenha cada triângulo.

---

## 🧠 **Parte 2 — Com matriz de transformação (usando GLM)**

### 🔹 Exercício 3 — Triângulos com clique e transformação

**Arquivo:** `Exec3.cpp`

**Solicitação:**

> Criar uma `struct Triangle` contendo posição e cor.
> Gerar um VAO base para um triângulo padrão.
> A cada clique do mouse, criar um novo triângulo na posição clicada,
> com cor RGB sorteada, utilizando **matrizes de transformação GLM**.

**Descrição da solução:**

* Um triângulo base é criado com a função `setupBaseTriangle()`.
* Cada clique do mouse chama `onMouseClick()`, que:

  * Converte a posição do clique para coordenadas normalizadas (-1 a 1);
  * Gera uma cor aleatória (`glm::vec3`);
  * Adiciona o triângulo à lista `triangleList`.
* A função `renderTrianglesWithTransform()` usa `glm::translate()` para posicionar cada triângulo e envia a cor via `uniform` para o shader.

**Interação:**

* 🖱️ Clique esquerdo → adiciona um novo triângulo na tela.
* 🎨 Cada triângulo possui uma cor aleatória.

---

## ⚙️ **Compilação e execução**

Pré-requisitos (macOS com Homebrew):

```bash
brew install glfw glm
```

Compilação:

```bash
make Exec1    # Exercício 1
make Exec2    # Exercício 2
make Exec3    # Exercício 3
```

Execução:

```bash
./Exec1
./Exec2
./Exec3
```

---

## 📚 **Bibliotecas utilizadas**

| Biblioteca | Função                                                      |
| ---------- | ----------------------------------------------------------- |
| **GLFW**   | Gerenciamento de janelas e eventos (teclado/mouse)          |
| **GLAD**   | Carregamento de funções OpenGL                              |
| **GLM**    | Matemática de gráficos (matrizes, vetores e transformações) |

---

## 🧑‍💻 **Resumo visual**

| Exercício | Descrição                                     | Interação       |
| --------- | --------------------------------------------- | --------------- |
| Exec1     | Triângulo fixo central                        | Nenhuma         |
| Exec2     | 5 triângulos fixos                            | Nenhuma         |
| Exec3     | Triângulos gerados com clique e cor aleatória | Clique esquerdo |

---

## ✍️ **Autor**

**Renan Pereira**
Disciplina: *Processamento Gráfico — Fundamentos*
Professora: *Rossana Baptista Queiroz*
Semestre: 2025/2
Universidade: **Unisinos**