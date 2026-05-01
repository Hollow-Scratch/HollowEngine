#include "Renderer.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/BufferLayout.hpp"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hollow {

std::unique_ptr<VertexArray> Renderer::s_VAO;
std::unique_ptr<VertexBuffer> Renderer::s_VBO;
std::unique_ptr<Shader> Renderer::s_Shader;

void Renderer::Init()
{
    glEnable(GL_DEPTH_TEST);

    s_Shader = std::make_unique<Shader>(
        "assets/shaders/basic.vert",
        "assets/shaders/basic.frag"
    );

    float vertices[] = {
         0.0f,  0.5f,
        -0.5f, -0.5f,
         0.5f, -0.5f
    };

    s_VAO = std::make_unique<VertexArray>();
    s_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

    BufferLayout layout;
    layout.PushFloat(2);

    s_VAO->AddBuffer(*s_VBO, layout);
}

void Renderer::Shutdown()
{
    s_Shader.reset();
    s_VAO.reset();
    s_VBO.reset();
}

void Renderer::Clear(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw()
{
    s_Shader->Bind();

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 view = glm::translate(glm::mat4(1.0f),
                                    glm::vec3(0.0f, 0.0f, -2.0f));

    float aspect = 800.0f / 600.0f;
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        aspect,
        0.1f,
        100.0f
    );

    glm::mat4 mvp = projection * view * model;

    s_Shader->SetMat4("u_MVP", glm::value_ptr(mvp));
    s_Shader->SetVec3("u_Color", 0.2f, 0.8f, 1.0f);

    s_VAO->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

}