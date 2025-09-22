
#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

// Protótipos
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
int setupShader();
int setupSpiral();

// Dimensões da janela
const GLuint WIDTH = 800, HEIGHT = 800;

// Código fonte do Vertex Shader
const GLchar *vertexShaderSource = R"(
 #version 400
 layout (location = 0) in vec3 position;
 void main() {
     gl_Position = vec4(position.x, position.y, position.z, 1.0);
 }
 )";

// Código fonte do Fragment Shader
const GLchar *fragmentShaderSource = R"(
 #version 400
 uniform vec4 inputColor;
 out vec4 color;
 void main() {
     color = inputColor;
 }
 )";

// Callback para tecla ESC
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// Compila e linka os shaders
int setupShader()
{
    GLint success;
    GLchar infoLog[512];

    // Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cerr << "ERRO::VERTEX_SHADER\n"
             << infoLog << endl;
    }

    // Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cerr << "ERRO::FRAGMENT_SHADER\n"
             << infoLog << endl;
    }

    // Programa de shader
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cerr << "ERRO::SHADER_PROGRAM\n"
             << infoLog << endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// Cria os pontos da espiral e configura VAO/VBO
int setupSpiral()
{
    // Parâmetros para uma espiral aberta (não fecha círculo)
    const int N = 400; // menos pontos para menos voltas
    float a = 0.0f;
    float max_radius = 90.0f / 400.0f;  // 0.225
    float b = max_radius / (N * 0.25f); // mais espaçado

    float vertices[N * 3];

    float theta_step = 0.35f; // aumenta o passo angular para abrir mais
    for (int i = 0; i < N; i++)
    {
        float theta = i * theta_step;
        float r = a + b * theta;
        if (r > max_radius)
            r = max_radius;
        float x = r * cosf(theta);
        float y = r * sinf(theta);

        vertices[i * 3 + 0] = x;
        vertices[i * 3 + 1] = y;
        vertices[i * 3 + 2] = 0.0f;
    }

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

// MAIN
int main()
{
    // Inicialização do GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Criação da janela
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Espiral", nullptr, nullptr);
    if (!window)
    {
        cerr << "Falha ao criar janela GLFW" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // Inicializa GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cerr << "Falha ao inicializar GLAD" << endl;
        return -1;
    }

    // Viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Shader + Geometria
    GLuint shaderID = setupShader();
    GLuint VAO = setupSpiral();
    GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");

    // Loop principal
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderID);
        glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f); // vermelho
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_STRIP, 0, 200);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();
    return 0;
}
