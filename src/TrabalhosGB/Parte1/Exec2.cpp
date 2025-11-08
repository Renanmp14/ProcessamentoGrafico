#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

// --- Shaders (iguais ao anterior) ---
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(0.2, 0.7, 0.9, 1.0); // azul claro
}
)";

// --- Função createTriangle (reaproveitada da Parte 1) ---
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

// --- Armazena os 5 triângulos ---
std::vector<GLuint> triangles;

void setupTriangles() {
    triangles.push_back(createTriangle(-0.8f, -0.5f, -0.6f, -0.5f, -0.7f, -0.2f));
    triangles.push_back(createTriangle(-0.4f, -0.5f, -0.2f, -0.5f, -0.3f, -0.2f));
    triangles.push_back(createTriangle(0.0f, -0.5f, 0.2f, -0.5f, 0.1f, -0.2f));
    triangles.push_back(createTriangle(0.4f, -0.5f, 0.6f, -0.5f, 0.5f, -0.2f));
    triangles.push_back(createTriangle(0.8f, -0.5f, 1.0f, -0.5f, 0.9f, -0.2f));
}

void renderTriangles(GLuint shaderProgram) {
    glUseProgram(shaderProgram);
    for (auto vao : triangles) {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}

// --- Função principal ---
int main() {
    // Inicializa GLFW
    if (!glfwInit()) {
        std::cerr << "Erro ao inicializar GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "5 Triângulos OpenGL", nullptr, nullptr);
    if (!window) {
        std::cerr << "Erro ao criar janela GLFW\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Inicializa GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Erro ao inicializar GLAD\n";
        return -1;
    }

    // Define viewport
    glViewport(0, 0, 800, 600);

    // --- Cria e compila shaders ---
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

    // --- Cria os 5 triângulos ---
    setupTriangles();

    // Define cor de fundo
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        renderTriangles(shaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpa memória
    for (auto vao : triangles)
        glDeleteVertexArrays(1, &vao);

    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}
