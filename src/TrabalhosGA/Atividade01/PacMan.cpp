#include <iostream>
#include <cmath>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr GLuint WIDTH = 800, HEIGHT = 800;
constexpr int PACMAN_SEGMENTS = 50;
constexpr float PACMAN_RADIUS = 0.5f;
constexpr float PACMAN_MOUTH_ANGLE = 0.2f * 3.1415926f; // ângulo da boca

// Vertex Shader
const char *vertexShaderSource = R"(
#version 400
layout (location = 0) in vec3 position;
void main() {
    gl_Position = vec4(position, 1.0);
}
)";

// Fragment Shader
const char *fragmentShaderSource = R"(
#version 400
uniform vec4 inputColor;
out vec4 color;
void main() {
    color = inputColor;
}
)";

// Callback de teclado
void key_callback(GLFWwindow *window, int key, int, int action, int) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// Compila e linka shaders, retorna o ID do programa
GLuint setupShader() {
    auto compileShader = [](GLenum type, const char *src) -> GLuint {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "Erro ao compilar shader: " << infoLog << std::endl;
        }
        return shader;
    };

    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Erro ao linkar programa: " << infoLog << std::endl;
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

// Gera a geometria do Pac-Man (círculo com boca)
void generatePacmanVertices(std::vector<float> &vertices) {
    float cx = 0.0f, cy = 0.0f, r = PACMAN_RADIUS;
    float start_angle = PACMAN_MOUTH_ANGLE;
    float end_angle = 2.0f * 3.1415926f - PACMAN_MOUTH_ANGLE;

    vertices.clear();
    vertices.push_back(cx);
    vertices.push_back(cy);
    vertices.push_back(0.0f);

    for (int i = 0; i <= PACMAN_SEGMENTS; ++i) {
        float theta = start_angle + (end_angle - start_angle) * float(i) / PACMAN_SEGMENTS;
        float x = cx + r * cosf(theta);
        float y = cy + r * sinf(theta);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
    }
}

// Cria VAO/VBO e retorna VAO
GLuint setupGeometry() {
    std::vector<float> vertices;
    generatePacmanVertices(vertices);

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Libera o VBO após o uso (VAO mantém referência)
    glDeleteBuffers(1, &VBO);

    return VAO;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Pac-Man", nullptr, nullptr);
    if (!window) {
        std::cerr << "Falha ao criar a janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Falha ao inicializar GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    GLuint shaderID = setupShader();
    GLuint VAO = setupGeometry();
    GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");

    glUseProgram(shaderID);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUniform4f(colorLoc, 1.0f, 1.0f, 0.0f, 1.0f); // amarelo Pac-Man
        glDrawArrays(GL_TRIANGLE_FAN, 0, PACMAN_SEGMENTS + 2);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderID);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}