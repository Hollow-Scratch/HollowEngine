#include "Renderer.hpp"
#include "Graphics/Shader.hpp"
#include <glad/gl.h>
#include <iostream>

namespace Hollow {

static unsigned int vao = 0;
static unsigned int vbo = 0;
static Shader* shader = nullptr;

void Renderer::Init()
{
    std::cout << "[Renderer] OpenGL Version: " << glGetString(GL_VERSION) << "\n";

    // Create shader
    shader = new Shader(
        "assets/shaders/basic.vert",
        "assets/shaders/basic.frag"
    );

    // Triangle vertices
    float vertices[] = {
         0.0f,  0.5f,
        -0.5f, -0.5f,
         0.5f, -0.5f
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
    shader->Bind();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

}