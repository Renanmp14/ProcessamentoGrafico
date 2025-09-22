#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr GLuint WIDTH = 800, HEIGHT = 800;
constexpr int OCTAGON_SEGMENTS = 8;

const char *vertexShaderSource = R"(
 #version 400
 layout (location = 0) in vec3 position;
 void main() {
     gl_Position = vec4(position, 1.0);
 }
)";

const char *fragmentShaderSource = R"(
 #version 400
 uniform vec4 inputColor;
 out vec4 color;
 void main() {
     color = inputColor;
 }
)";

void key_callback(GLFWwindow *window, int key, int, int action, int) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

GLuint compileShader(GLenum type, const char *source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Erro ao compilar shader: " << infoLog << std::endl;
    }
    return shader;
}

GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    GLint success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Erro ao linkar programa: " << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

void setupGeometry(GLuint &VAO, GLuint &VBO) {
    float cx = 0.0f, cy = 0.0f, r = 0.5f;
    float vertices[(OCTAGON_SEGMENTS + 2) * 3];
    // Centro
    vertices[0] = cx;
    vertices[1] = cy;
    vertices[2] = 0.0f;
    // Pontos do octágono
    for (int i = 0; i <= OCTAGON_SEGMENTS; ++i) {
        float theta = 2.0f * 3.1415926f * float(i) / float(OCTAGON_SEGMENTS);
        float x = cx + r * cosf(theta);
        float y = cy + r * sinf(theta);
        vertices[(i + 1) * 3 + 0] = x;
        vertices[(i + 1) * 3 + 1] = y;
        vertices[(i + 1) * 3 + 2] = 0.0f;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Erro ao inicializar GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Octágono", nullptr, nullptr);
    if (!window) {
        std::cerr << "Falha ao criar a janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Falha ao inicializar GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    GLuint shaderID = createShaderProgram();
    GLuint VAO, VBO;
    setupGeometry(VAO, VBO);

    GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
    glUseProgram(shaderID);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUniform4f(colorLoc, 0.2f, 0.8f, 1.0f, 1.0f); // azul claro
        glDrawArrays(GL_TRIANGLE_FAN, 0, OCTAGON_SEGMENTS + 2);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}