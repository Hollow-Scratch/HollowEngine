#include "Renderer.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/BufferLayout.hpp"

#include <glad/gl.h>
#include <memory>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hollow {

static std::unique_ptr<VertexArray> vao;
static std::unique_ptr<VertexBuffer> vbo;
static std::unique_ptr<Shader> shader;

void Renderer::Init()
{
    shader = std::make_unique<Shader>(
        "assets/shaders/basic.vert",
        "assets/shaders/basic.frag"
    );

    float vertices[] = {
         0.0f,  0.5f,
        -0.5f, -0.5f,
         0.5f, -0.5f
    };

    vao = std::make_unique<VertexArray>();
    vbo = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

    BufferLayout layout;
    layout.PushFloat(2);

    vao->AddBuffer(*vbo, layout);
}

void Renderer::Clear(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw()
{
    shader->Bind();

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 view = glm::translate(glm::mat4(1.0f),
                                    glm::vec3(0.0f, 0.0f, -2.0f));

    float aspect = 800.0f / 600.0f; // change later to window size
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        aspect,
        0.1f,
        100.0f
    );

    glm::mat4 mvp = projection * view * model;

    shader->SetMat4("u_MVP", glm::value_ptr(mvp));

    // color still works
    shader->SetVec3("u_Color", 0.2f, 0.8f, 1.0f);

    vao->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

}