#include "Renderer.hpp"
#include "Graphics/Shader.hpp"
#include "Renderer/Camera.hpp"
#include "ECS/Registry.hpp"
#include "Core/Application.hpp"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hollow {

std::unique_ptr<Shader> Renderer::s_Shader;

void Renderer::Init()
{
    glEnable(GL_DEPTH_TEST);

    s_Shader = std::make_unique<Shader>(
        "assets/shaders/basic.vert",
        "assets/shaders/basic.frag"
    );
}

void Renderer::Shutdown()
{
    if (s_Shader)
        s_Shader->Destroy();

    s_Shader.reset();
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

    float time = Application::Get().GetTime().getElapsedTime();
    s_Shader->SetFloat("u_Time", time);

    for (auto& [entity, transform] : registry.GetTransforms())
    {
        auto& meshes = registry.GetMeshes();

        if (meshes.find(entity) == meshes.end())
            continue;

        auto& mesh = meshes.at(entity);

        if (!mesh.VAO || !mesh.EBO)
            continue;

        mesh.VAO->Bind();
        mesh.EBO->Bind();

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, transform.Position);
        model = glm::rotate(model, glm::radians(transform.Rotation.x), {1,0,0});
        model = glm::rotate(model, glm::radians(transform.Rotation.y), {0,1,0});
        model = glm::rotate(model, glm::radians(transform.Rotation.z), {0,0,1});
        model = glm::scale(model, transform.Scale);

        glm::mat4 mvp = projection * view * model;

        s_Shader->SetMat4("u_MVP", glm::value_ptr(mvp));

        glDrawElements(GL_TRIANGLES, mesh.IndexCount, GL_UNSIGNED_INT, nullptr);
    }
}

}