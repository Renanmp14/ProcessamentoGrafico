#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cstdlib>
#include <iostream>

// --- Shaders com suporte a transformação e cor ---
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 color;
void main() {
    FragColor = vec4(color, 1.0);
}
)";

// --- Estrutura de dados dos triângulos ---
struct Triangle {
    glm::vec2 position;
    glm::vec3 color;
};

GLuint triangleVAO;
std::vector<Triangle> triangleList;

// --- Função para criar um triângulo base (VAO) ---
GLuint createTriangle(float x0, float y0, float x1, float y1, float x2, float y2) {
    float vertices[] = {
        x0, y0, 0.0f,
        x1, y1, 0.0f,
        x2, y2, 0.0f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

// --- Cria o triângulo base padrão ---
void setupBaseTriangle() {
    triangleVAO = createTriangle(-0.1f, -0.1f, 0.1f, -0.1f, 0.0f, 0.1f);
}

// --- Gera um triângulo na posição do clique com cor aleatória ---
void onMouseClick(float x, float y) {
    // converte coordenadas de tela (pixel) para coordenadas normalizadas (-1 a 1)
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    float normX = (x / width) * 2.0f - 1.0f;
    float normY = 1.0f - (y / height) * 2.0f; // inverte o Y

    Triangle t;
    t.position = glm::vec2(normX, normY);
    t.color = glm::vec3(
        static_cast<float>(rand()) / RAND_MAX,
        static_cast<float>(rand()) / RAND_MAX,
        static_cast<float>(rand()) / RAND_MAX
    );
    triangleList.push_back(t);
}

// --- Renderiza todos os triângulos com matriz de transformação ---
void renderTrianglesWithTransform(GLuint shaderProgram) {
    glUseProgram(shaderProgram);
    glBindVertexArray(triangleVAO);

    for (const auto& t : triangleList) {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(t.position, 0.0f));

        GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        GLuint colorLoc = glGetUniformLocation(shaderProgram, "color");
        glUniform3fv(colorLoc, 1, glm::value_ptr(t.color));

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}

// --- Função callback de mouse ---
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        onMouseClick(static_cast<float>(xpos), static_cast<float>(ypos));
    }
}

// --- Programa principal ---
int main() {
    if (!glfwInit()) {
        std::cerr << "Erro ao inicializar GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Triângulos com GLM", nullptr, nullptr);
    if (!window) {
        std::cerr << "Erro ao criar janela GLFW\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Erro ao inicializar GLAD\n";
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    // --- Shaders ---
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Cria o triângulo base
    setupBaseTriangle();

    // Define cor de fundo
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        renderTrianglesWithTransform(shaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &triangleVAO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}
