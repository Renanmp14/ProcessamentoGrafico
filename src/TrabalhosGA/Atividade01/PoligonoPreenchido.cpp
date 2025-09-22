#include <iostream>
#include <string>
#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

// Constantes
const GLuint WIDTH = 800, HEIGHT = 600;

// Shaders
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

// Prototipagem
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
GLuint createShader(GLenum type, const char *source);
GLuint createShaderProgram();
GLuint setupGeometry(GLuint &VBO);

// Função principal
int main() {
    if (!glfwInit()) {
        cerr << "Erro ao inicializar GLFW" << endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Poligono preenchido!", nullptr, nullptr);
    if (!window) {
        cerr << "Falha ao criar a janela GLFW" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cerr << "Falha ao inicializar GLAD" << endl;
        glfwTerminate();
        return -1;
    }

    cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    GLuint shaderID = createShaderProgram();
    GLuint VBO, VAO = setupGeometry(VBO);
    GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");

    glUseProgram(shaderID);

    double prev_s = glfwGetTime();
    double title_countdown_s = 0.1;

    while (!glfwWindowShouldClose(window)) {
        double curr_s = glfwGetTime();
        double elapsed_s = curr_s - prev_s;
        prev_s = curr_s;
        title_countdown_s -= elapsed_s;
        if (title_countdown_s <= 0.0 && elapsed_s > 0.0) {
            double fps = 1.0 / elapsed_s;
            char tmp[128];
            snprintf(tmp, sizeof(tmp), "Poligono preenchido! \tFPS %.2lf", fps);
            glfwSetWindowTitle(window, tmp);
            title_countdown_s = 0.1;
        }

        glfwPollEvents();
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glLineWidth(10);
        glPointSize(20);
        glBindVertexArray(VAO);

        glUniform4f(colorLoc, 0.6f, 1.0f, 0.6f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUniform4f(colorLoc, 0.8f, 0.8f, 0.8f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 3, 3);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

// Funções auxiliares

void key_callback(GLFWwindow *window, int key, int, int action, int) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

GLuint createShader(GLenum type, const char *source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        cerr << "Erro ao compilar shader: " << infoLog << endl;
    }
    return shader;
}

GLuint createShaderProgram() {
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        cerr << "Erro ao linkar programa: " << infoLog << endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

GLuint setupGeometry(GLuint &VBO) {
    GLfloat vertices[] = {
        -0.8f, -0.5f, 0.0f, -0.2f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f,
         0.2f, -0.5f, 0.0f,  0.8f, -0.5f, 0.0f,  0.5f, 0.5f, 0.0f
    };
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return VAO;
}