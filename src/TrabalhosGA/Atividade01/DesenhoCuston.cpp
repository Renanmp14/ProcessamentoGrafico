#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Callback para ajustar viewport
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Vertex Shader
const char *vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;

    out vec3 ourColor;

    void main() {
        gl_Position = vec4(aPos, 1.0);
        ourColor = aColor;
    }
)glsl";

// Fragment Shader
const char *fragmentShaderSource = R"glsl(
    #version 330 core
    in vec3 ourColor;
    out vec4 FragColor;

    void main() {
        FragColor = vec4(ourColor, 1.0);
    }
)glsl";

int main()
{
    // Inicialização
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Casa", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Erro ao criar janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Erro ao inicializar GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Compilação shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vértices da casa
    float vertices[] = {
        // ===== Telhado (triângulo vermelho) =====
        0.0f,
        0.7f,
        0.0f,
        0.8f,
        0.0f,
        0.0f, // topo
        -0.6f,
        0.3f,
        0.0f,
        0.8f,
        0.0f,
        0.0f, // esquerda
        0.6f,
        0.3f,
        0.0f,
        0.8f,
        0.0f,
        0.0f, // direita

        // ===== Corpo (quadrado branco) =====
        -0.6f,
        0.3f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        0.6f,
        0.3f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        0.6f,
        -0.5f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        -0.6f,
        -0.5f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,

        // ===== Porta (retângulo marrom) =====
        -0.15f,
        -0.5f,
        0.0f,
        0.4f,
        0.2f,
        0.2f,
        0.15f,
        -0.5f,
        0.0f,
        0.4f,
        0.2f,
        0.2f,
        0.15f,
        -0.1f,
        0.0f,
        0.4f,
        0.2f,
        0.2f,
        -0.15f,
        -0.1f,
        0.0f,
        0.4f,
        0.2f,
        0.2f,

        // ===== Janela (quadrado amarelo) =====
        -0.35f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        -0.15f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        -0.15f,
        0.2f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        -0.35f,
        0.2f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,

        // ===== Chão (retângulo laranja) =====
        -1.0f,
        -0.5f,
        0.0f,
        1.0f,
        0.5f,
        0.0f,
        1.0f,
        -0.5f,
        0.0f,
        1.0f,
        0.5f,
        0.0f,
        1.0f,
        -0.7f,
        0.0f,
        1.0f,
        0.5f,
        0.0f,
        -1.0f,
        -0.7f,
        0.0f,
        1.0f,
        0.5f,
        0.0f,
    };

    unsigned int indices[] = {
        // Telhado
        0, 1, 2,
        // Corpo
        3, 4, 5,
        3, 5, 6,
        // Porta
        7, 8, 9,
        7, 9, 10,
        // Janela
        11, 12, 13,
        11, 13, 14,
        // Chão
        15, 16, 17,
        15, 17, 18};

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Loop principal
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.7f, 0.9f, 1.0f, 1.0f); // fundo azul claro
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}