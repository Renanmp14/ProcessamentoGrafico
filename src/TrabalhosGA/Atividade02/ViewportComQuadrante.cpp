#include <iostream>
#include <cmath>
#include <array>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr GLuint WIDTH = 800, HEIGHT = 600;
constexpr int CIRCLE_SEGMENTS = 100;
constexpr float CX = 400.0f, CY = 300.0f, R = 100.0f;

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

void key_callback(GLFWwindow *window, int key, int, int action, int)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

GLuint setupShader()
{
    auto compileShader = [](GLenum type, const GLchar *src) -> GLuint {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
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
        GLchar infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Erro ao linkar programa: " << infoLog << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

GLuint setupCircleVAO()
{
    std::array<float, (CIRCLE_SEGMENTS + 2) * 3> vertices;
    vertices[0] = CX;
    vertices[1] = CY;
    vertices[2] = 0.0f;
    for (int i = 0; i <= CIRCLE_SEGMENTS; ++i) {
        float theta = 2.0f * M_PI * float(i) / float(CIRCLE_SEGMENTS);
        vertices[(i + 1) * 3 + 0] = CX + R * cosf(theta);
        vertices[(i + 1) * 3 + 1] = CY + R * sinf(theta);
        vertices[(i + 1) * 3 + 2] = 0.0f;
    }
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO); // VAO mantém referência
    return VAO;
}

struct LineVAO {
    GLuint vao, vbo;
};

LineVAO createLineVAO(float x1, float y1, float x2, float y2)
{
    float line[] = {x1, y1, 0.0f, x2, y2, 0.0f};
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return {vao, vbo};
}

void drawDashedLine(GLuint shaderID, GLint colorLoc, GLint widthLoc, GLint heightLoc,
                    float x1, float y1, float x2, float y2, float dash, float gap)
{
    float dx = x2 - x1, dy = y2 - y1;
    float length = std::sqrt(dx * dx + dy * dy);
    float vx = dx / length, vy = dy / length;
    for (float d = 0; d < length; d += dash + gap) {
        float segStart = d;
        float segEnd = std::min(d + dash, length);
        float sx = x1 + vx * segStart, sy = y1 + vy * segStart;
        float ex = x1 + vx * segEnd, ey = y1 + vy * segEnd;
        LineVAO lvao = createLineVAO(sx, sy, ex, ey);
        glUseProgram(shaderID);
        glUniform4f(colorLoc, 0.5f, 0.5f, 0.5f, 1.0f);
        glUniform1f(widthLoc, WIDTH);
        glUniform1f(heightLoc, HEIGHT);
        glBindVertexArray(lvao.vao);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
        glDeleteBuffers(1, &lvao.vbo);
        glDeleteVertexArrays(1, &lvao.vao);
    }
}

int main()
{
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Circulo no quadrante superior direito", nullptr, nullptr);
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

    GLuint shaderID = setupShader();
    GLuint circleVAO = setupCircleVAO();

    GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
    GLint widthLoc = glGetUniformLocation(shaderID, "u_width");
    GLint heightLoc = glGetUniformLocation(shaderID, "u_height");

    constexpr float dash = 10.0f, gap = 10.0f;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glViewport(0, 0, WIDTH, HEIGHT);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Linhas pontilhadas dos quadrantes
        drawDashedLine(shaderID, colorLoc, widthLoc, heightLoc, 400, 0, 400, HEIGHT, dash, gap); // vertical
        drawDashedLine(shaderID, colorLoc, widthLoc, heightLoc, 0, 300, WIDTH, 300, dash, gap); // horizontal

        // Círculo no quadrante superior direito
        glViewport(WIDTH / 2, HEIGHT / 2, WIDTH / 2, HEIGHT / 2);
        glUseProgram(shaderID);
        glUniform1f(widthLoc, WIDTH);
        glUniform1f(heightLoc, HEIGHT);
        glUniform4f(colorLoc, 0.2f, 0.8f, 1.0f, 1.0f);
        glBindVertexArray(circleVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_SEGMENTS + 2);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &circleVAO);
    glfwTerminate();
    return 0;
}