#include "Renderer.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/BufferLayout.hpp"
#include "Renderer/Camera.hpp"
#include "ECS/Registry.hpp"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hollow {

std::unique_ptr<VertexArray> Renderer::s_VAO;
std::unique_ptr<VertexBuffer> Renderer::s_VBO;
std::unique_ptr<IndexBuffer> Renderer::s_EBO;
std::unique_ptr<Shader> Renderer::s_Shader;

void Renderer::Init()
{
    glEnable(GL_DEPTH_TEST);

    s_Shader = std::make_unique<Shader>(
        "assets/shaders/basic.vert",
        "assets/shaders/basic.frag"
    );

    float vertices[] = {
        -0.5f,-0.5f,-0.5f,
         0.5f,-0.5f,-0.5f,
         0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f,

        -0.5f,-0.5f, 0.5f,
         0.5f,-0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f
    };

    unsigned int indices[] = {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        0,4,7, 7,3,0,
        1,5,6, 6,2,1,
        0,1,5, 5,4,0,
        3,2,6, 6,7,3
    };

    s_VAO = std::make_unique<VertexArray>();
    s_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
    s_EBO = std::make_unique<IndexBuffer>(indices, 36);

    BufferLayout layout;
    layout.PushFloat(3);

    s_VAO->AddBuffer(*s_VBO, layout);
    s_EBO->Bind();
}

void Renderer::Shutdown()
{
    if (s_Shader)
        s_Shader->Destroy();

    if (s_VAO)
        s_VAO->Destroy();

    if (s_VBO)
        s_VBO->Destroy();

    if (s_EBO)
        s_EBO->Destroy();

    s_Shader.reset();
    s_VAO.reset();
    s_VBO.reset();
    s_EBO.reset();
}

void Renderer::Clear(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(Registry& registry, float width, float height)
{
    Camera* activeCamera = nullptr;

    for (auto& [entity, cam] : registry.GetCameras())
    {
        if (cam.Primary)
        {
            activeCamera = &cam.CameraData;
            break;
        }
    }

    if (!activeCamera)
        return;

    s_Shader->Bind();

    glm::mat4 projection = activeCamera->GetProjectionMatrix();
    glm::mat4 view       = activeCamera->GetViewMatrix();

    s_VAO->Bind();

    for (auto& [entity, transform] : registry.GetTransforms())
    {
        if (registry.GetMeshes().find(entity) == registry.GetMeshes().end())
            continue;

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, transform.Position);
        model = glm::rotate(model, glm::radians(transform.Rotation.x), {1,0,0});
        model = glm::rotate(model, glm::radians(transform.Rotation.y), {0,1,0});
        model = glm::rotate(model, glm::radians(transform.Rotation.z), {0,0,1});
        model = glm::scale(model, transform.Scale);

        glm::mat4 mvp = projection * view * model;

        s_Shader->SetMat4("u_MVP", glm::value_ptr(mvp));
        s_Shader->SetVec3("u_Color", 0.2f, 0.8f, 1.0f);

        glDrawElements(GL_TRIANGLES, s_EBO->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
}

}