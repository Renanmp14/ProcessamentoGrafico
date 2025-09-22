#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr GLuint WIDTH = 800, HEIGHT = 800;
constexpr int PIZZA_SEGMENTS = 50;
constexpr float PIZZA_RADIUS = 0.5f;
constexpr float START_ANGLE = 0.0f;
constexpr float END_ANGLE = 1.0f * 3.1415926f / 3.0f; // 60 graus

void key_callback(GLFWwindow *window, int key, int, int action, int)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

GLuint setupShader()
{
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

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Erro ao linkar programa: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

GLuint setupGeometry()
{
    float vertices[(PIZZA_SEGMENTS + 2) * 3];
    vertices[0] = 0.0f; vertices[1] = 0.0f; vertices[2] = 0.0f;
    for (int i = 0; i <= PIZZA_SEGMENTS; ++i) {
        float theta = START_ANGLE + (END_ANGLE - START_ANGLE) * float(i) / float(PIZZA_SEGMENTS);
        vertices[(i + 1) * 3 + 0] = PIZZA_RADIUS * cosf(theta);
        vertices[(i + 1) * 3 + 1] = PIZZA_RADIUS * sinf(theta);
        vertices[(i + 1) * 3 + 2] = 0.0f;
    }

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO); // Libera o VBO após configurar o VAO

    return VAO;
}

int main()
{
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Fatia pizza", nullptr, nullptr);
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

    GLuint shaderID = setupShader();
    GLuint VAO = setupGeometry();
    GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");

    glUseProgram(shaderID);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUniform4f(colorLoc, 0.2f, 0.8f, 1.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 0, PIZZA_SEGMENTS + 2);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();
    return 0;
}