# Resolução das questões da Lista 02

## 1. Modifique a janela do mundo (window/ortho) para os limites: xmin=-10, xmax=10, ymin=-10, ymax=10.

O arquivo para o desenho da atividade se encontra: 

```
src/TrabalhosGA/Atividade02
```

- [Ex01](https://github.com/Renanmp14/ProcessamentoGrafico/blob/main/src/TrabalhosGA/Atividade02/Ex01.cpp)  

---

## 2. Agora modifique para: xmin=0, xmax=800, ymin=600, ymax=0.

```
src/TrabalhosGA/Atividade02
```

- [Ex02](https://github.com/Renanmp14/ProcessamentoGrafico/blob/main/src/TrabalhosGA/Atividade02/Ex02.cpp)  

---

## 3. Utilizando a câmera 2D do exercício anterior, desenhe algo na tela. O que acontece quando posicionamos os objetos? Por que é útil essa configuração?


### 1️⃣ Desenhando algo na tela

Quando você usa a câmera 2D, você normalmente faz algo assim:

```cpp
// Exemplo conceitual em OpenGL
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0, largura_tela, 0, altura_tela); // define "janela" 2D

glMatrixMode(GL_MODELVIEW);
glLoadIdentity();

// Desenhando um retângulo
glBegin(GL_QUADS);
    glVertex2f(100, 100);
    glVertex2f(200, 100);
    glVertex2f(200, 200);
    glVertex2f(100, 200);
glEnd();
```

* Aqui, `(0,0)` normalmente fica no canto inferior esquerdo da tela.
* As coordenadas dos objetos são **diretamente mapeadas para a tela**.

---

### 2️⃣ O que acontece quando posicionamos objetos?

* Se você muda as coordenadas de um objeto (ex.: `glVertex2f(300, 300)`), ele aparece em **outra posição da tela**.
* Se você desenhar vários objetos, eles **não se sobrepõem** se as coordenadas forem diferentes.
* O “movimento” de objetos na tela é feito **alterando suas coordenadas X e Y**.

Isso acontece porque a câmera 2D não tem perspectiva: **não há profundidade real**, apenas posições X e Y.

---

### 3️⃣ Por que é útil essa configuração?

* **Simplicidade**: não precisa pensar em profundidade (Z), apenas em altura e largura.
* **Controle preciso**: você sabe exatamente onde o objeto aparecerá na tela.
* **Interface gráfica**: útil para jogos 2D, menus, HUDs, mapas etc.
* **Transformações fáceis**: mover, escalar ou rotacionar objetos 2D é direto, sem complicações de projeção 3D.

---

💡 **Resumo**:
Ao posicionar objetos na câmera 2D, você está definindo **a coordenada X e Y na tela**, e isso é útil porque permite desenhar de forma intuitiva e precisa em um plano bidimensional, ideal para interfaces e jogos 2D.

Desenho: -[EX03](https://github.com/Renanmp14/ProcessamentoGrafico/blob/main/src/TrabalhosGA/Atividade02/Ex03.cpp)  

---

## 4. Modifique o viewport para desenhar a cena apenas no seguinte quadrante da janela da aplicação:

O desenho correspondente está em:

```
src/TrabalhosGA/Atividade02
```

- [ViewportComQuadrante](https://github.com/Renanmp14/ProcessamentoGrafico/blob/main/src/TrabalhosGA/Atividade02/ViewportComQuadrante.cpp)

---

## 5. Agora, desenhe a mesma cena nos 4 quadrantes.

O desenho correspondente está em:

```
src/TrabalhosGA/Atividade02
```

- [ViewportCom4Quadrante](https://github.com/Renanmp14/ProcessamentoGrafico/blob/main/src/TrabalhosGA/Atividade02/ViewportCom4Quadrante.cpp)

---

## 6. Crie triângulos a partir do clique do mouse. Para isso, seu programa deve atender os seguintes requisitos:

### Requisitos

1. **Vértices via clique**
   - Cada clique do mouse adiciona **apenas 1 vértice**.
   
2. **Formação de triângulos**
   - A cada **3 vértices criados**, o programa forma um triângulo.

3. **Cores diferentes**
   - Cada novo triângulo deve ter uma **cor única**, diferente dos anteriores.

---

O desenho correspondente está em:

```
src/TrabalhosGA/Atividade02
```

- [TrianguloComClique](https://github.com/Renanmp14/ProcessamentoGrafico/blob/main/src/TrabalhosGA/Atividade02/TrianguloComClique.cpp)
