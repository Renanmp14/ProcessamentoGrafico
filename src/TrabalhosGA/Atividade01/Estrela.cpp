#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr GLuint WIDTH = 800, HEIGHT = 800;
constexpr int STAR_POINTS = 5; // Número de pontas da estrela

// Vertex Shader (GLSL)
const char *vertexShaderSource = R"(
#version 400
layout (location = 0) in vec3 position;
void main() {
    gl_Position = vec4(position, 1.0);
}
)";

// Fragment Shader (GLSL)
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

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Erro ao linkar programa: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

// Cria VAO/VBO para a estrela, retorna o VAO
GLuint setupGeometry() {
    constexpr float cx = 0.0f, cy = 0.0f;
    constexpr float r_outer = 0.5f, r_inner = 0.22f;
    constexpr int n = STAR_POINTS * 2;
    float vertices[(n + 2) * 3];

    // Centro (opcional para TRIANGLE_FAN)
    vertices[0] = cx;
    vertices[1] = cy;
    vertices[2] = 0.0f;

    for (int i = 0; i <= n; ++i) {
        float angle = 2.0f * M_PI * float(i) / float(n);
        float r = (i % 2 == 0) ? r_outer : r_inner;
        float x = cx + r * cosf(angle - M_PI_2);
        float y = cy + r * sinf(angle - M_PI_2);
        vertices[(i + 1) * 3 + 0] = x;
        vertices[(i + 1) * 3 + 1] = y;
        vertices[(i + 1) * 3 + 2] = 0.0f;
    }

    GLuint VBO = 0, VAO = 0;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Libera o VBO após uso
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

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Estrela", nullptr, nullptr);
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
        glUniform4f(colorLoc, 0.2f, 0.8f, 1.0f, 1.0f); // azul claro
        glDrawArrays(GL_TRIANGLE_FAN, 0, STAR_POINTS * 2 + 2);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderID);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}