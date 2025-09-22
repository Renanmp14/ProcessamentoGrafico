
#include <iostream>
#include <vector>
#include <random>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

// Dimensões da janela (e da "janela do mundo")
const GLuint WIDTH = 800, HEIGHT = 600;

// Estrutura para armazenar um vértice 3D
struct Vertex
{
    float x, y, z;
};

// Estrutura para armazenar um triângulo (3 vértices + cor)
struct Triangle
{
    Vertex v[3];
    float color[4];
};

// Vetor para armazenar os vértices temporários (a cada clique)
vector<Vertex> currentVertices;
// Vetor para armazenar todos os triângulos já criados
vector<Triangle> triangles;

// Gera uma cor aleatória (RGBA) para cada triângulo
void randomColor(float color[4])
{
    static std::mt19937 rng{std::random_device{}()};
    static std::uniform_real_distribution<float> dist(0.2f, 1.0f);
    color[0] = dist(rng);
    color[1] = dist(rng);
    color[2] = dist(rng);
    color[3] = 1.0f;
}

// Callback de clique do mouse: adiciona vértices e cria triângulo a cada 3 cliques
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        // Inverter y para coordenada de tela (origem no canto inferior esquerdo)
        ypos = HEIGHT - ypos;
        // Adiciona o vértice clicado
        currentVertices.push_back({(float)xpos, (float)ypos, 0.0f});
        // Se já temos 3 vértices, cria um triângulo
        if (currentVertices.size() == 3)
        {
            Triangle t;
            for (int i = 0; i < 3; ++i)
                t.v[i] = currentVertices[i];
            randomColor(t.color);
            triangles.push_back(t);
            currentVertices.clear();
        }
    }
}

// Vertex Shader: converte coordenadas de pixel para NDC
const GLchar *vertexShaderSource = R"(
#version 400
layout (location = 0) in vec3 position;
void main() {
    float x_ndc = (position.x / 800.0) * 2.0 - 1.0;
    float y_ndc = (position.y / 600.0) * 2.0 - 1.0;
    gl_Position = vec4(x_ndc, y_ndc, position.z, 1.0);
}
)";

// Fragment Shader: recebe cor via uniform
const GLchar *fragmentShaderSource = R"(
#version 400
uniform vec4 inputColor;
out vec4 color;
void main() {
    color = inputColor;
}
)";

// Compila e linka os shaders, retorna o ID do programa
GLuint setupShader()
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "Vertex Shader Error: " << infoLog << endl;
    }
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "Fragment Shader Error: " << infoLog << endl;
    }
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "Shader Program Link Error: " << infoLog << endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

int main()
{
    // Inicialização da GLFW e criação da janela
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Triângulos com Clique", nullptr, nullptr);
    if (!window)
    {
        cerr << "Falha ao criar janela GLFW" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cerr << "Falha ao inicializar GLAD" << endl;
        return -1;
    }
    // Registra o callback de clique do mouse
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // Compila e ativa o shader
    GLuint shaderID = setupShader();
    GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
    glUseProgram(shaderID);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        // Limpa a tela
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Desenha todos os triângulos já criados
        for (const auto &t : triangles)
        {
            GLuint VAO, VBO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(t.v), t.v, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
            glEnableVertexAttribArray(0);
            glUniform4fv(colorLoc, 1, t.color); // cor única do triângulo
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            glDeleteBuffers(1, &VBO);
            glDeleteVertexArrays(1, &VAO);
        }
        // Desenha os pontos dos vértices atuais (ainda não formam triângulo)
        if (!currentVertices.empty())
        {
            GLuint VAO, VBO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * currentVertices.size(), currentVertices.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
            glEnableVertexAttribArray(0);
            glUniform4f(colorLoc, 1.0f, 1.0f, 0.0f, 1.0f); // amarelo
            glPointSize(8.0f);
            glDrawArrays(GL_POINTS, 0, currentVertices.size());
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            glDeleteBuffers(1, &VBO);
            glDeleteVertexArrays(1, &VAO);
        }
        // Troca os buffers da tela
        glfwSwapBuffers(window);
    }
    // Finaliza GLFW
    glfwTerminate();
    return 0;
}