#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const GLuint WIDTH = 800, HEIGHT = 800;
const int CIRCLE_SEGMENTS = 100;

// Vertex Shader
const GLchar *vertexShaderSource = R"(
 #version 400
 layout (location = 0) in vec3 position;
 uniform float u_width;
 uniform float u_height;
 void main() {
     float x_ndc = (position.x / u_width) * 2.0 - 1.0;
     float y_ndc = (position.y / u_height) * 2.0 - 1.0;
     gl_Position = vec4(x_ndc, y_ndc, position.z, 1.0);
 }
 )";

// Fragment Shader
const GLchar *fragmentShaderSource = R"(
 #version 400
 uniform vec4 inputColor;
 out vec4 color;
 void main() {
     color = inputColor;
 }
 )";

// Callback para fechar janela com ESC
void key_callback(GLFWwindow *window, int key, int, int action, int)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// Compila e linka shaders
GLuint setupShader()
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Erro Vertex Shader:\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Erro Fragment Shader:\n" << infoLog << std::endl;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Erro Link Program:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

// Cria VAO/VBO para círculo centralizado
void createCircleVAO(GLuint &VAO, GLuint &VBO)
{
    float cx = 400.0f, cy = 400.0f, r = 100.0f;
    float vertices[(CIRCLE_SEGMENTS + 2) * 3];
    vertices[0] = cx;
    vertices[1] = cy;
    vertices[2] = 0.0f;
    for (int i = 0; i <= CIRCLE_SEGMENTS; ++i)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(CIRCLE_SEGMENTS);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Cria VAO/VBO para retângulo no canto superior esquerdo
void createRectVAO(GLuint &VAO, GLuint &VBO)
{
    float rect[] = {
        0.0f, 0.0f, 0.0f,
        100.0f, 0.0f, 0.0f,
        100.0f, 100.0f, 0.0f,
        0.0f, 100.0f, 0.0f};
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Circulo", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Falha ao criar a janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Falha ao inicializar GLAD" << std::endl;
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    GLuint shaderID = setupShader();

    // Cria VAO/VBO do círculo e do retângulo uma única vez
    GLuint circleVAO, circleVBO, rectVAO, rectVBO;
    createCircleVAO(circleVAO, circleVBO);
    createRectVAO(rectVAO, rectVBO);

    GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
    GLint widthLoc = glGetUniformLocation(shaderID, "u_width");
    GLint heightLoc = glGetUniformLocation(shaderID, "u_height");

    glUseProgram(shaderID);
    glUniform1f(widthLoc, 800.0f);
    glUniform1f(heightLoc, 800.0f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Desenha círculo azul claro no centro
        glBindVertexArray(circleVAO);
        glUniform4f(colorLoc, 0.2f, 0.8f, 1.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_SEGMENTS + 2);

        // Desenha retângulo vermelho no canto superior esquerdo
        glBindVertexArray(rectVAO);
        glUniform4f(colorLoc, 1.0f, 0.2f, 0.2f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    // Libera recursos
    glDeleteVertexArrays(1, &circleVAO);
    glDeleteBuffers(1, &circleVBO);
    glDeleteVertexArrays(1, &rectVAO);
    glDeleteBuffers(1, &rectVBO);

    glfwTerminate();
    return 0;
}