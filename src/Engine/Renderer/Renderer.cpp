#include "Renderer.hpp"
#include <glad/gl.h>
#include <iostream>

namespace Raycaster {

static unsigned int vao = 0;
static unsigned int vbo = 0;
static unsigned int shaderProgram = 0;

static unsigned int CompileShader(unsigned int type, const char* src)
{
    if (glad_glCreateShader == nullptr) {
        std::cerr << "[OpenGL] glCreateShader is NULL (GLAD not loaded?)\n";
        std::abort();
    }

    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info[512];
        glGetShaderInfoLog(id, 512, nullptr, info);
        std::cerr << "[Shader] Compile error:\n" << info << "\n";
    }

    return id;
}

static void CreateShader()
{
    const char* vs = R"(
        #version 330 core
        layout(location = 0) in vec2 aPos;
        void main()
        {
            gl_Position = vec4(aPos, 0.0, 1.0);
        }
    )";

    const char* fs = R"(
        #version 330 core
        out vec4 FragColor;
        void main()
        {
            FragColor = vec4(1.0, 1.0, 1.0, 1.0);
        }
    )";

    unsigned int vertex = CompileShader(GL_VERTEX_SHADER, vs);
    unsigned int fragment = CompileShader(GL_FRAGMENT_SHADER, fs);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char info[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, info);
        std::cerr << "[Shader] Link error:\n" << info << "\n";
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Renderer::Init()
{
    if (glad_glCreateShader == nullptr) {
        std::cerr << "[Renderer] OpenGL not initialized before Renderer::Init()\n";
        std::abort();
    }

    std::cout << "[Renderer] OpenGL Version: " << glGetString(GL_VERSION) << "\n";

    CreateShader();

    float vertices[] = {
        -0.5f, 0.0f,
         0.5f, 0.0f
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Renderer::Clear(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw()
{
    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 2);
}

}
