#include "Raycaster.h"
#include "ECS/Registry.hpp"
#include "Core/Window.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/BufferLayout.hpp"

#include <iostream>

namespace Hollow {

class SandboxApp : public Application {
private:
    Registry m_Registry;

    Entity m_CameraEntity;

    float m_Yaw   = -90.0f;
    float m_Pitch =  0.0f;

protected:
    void OnInit() override {
        std::cout << "[Sandbox] Init\n";

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

        auto e1 = m_Registry.CreateEntity();
        auto& t1 = m_Registry.AddTransform(e1);
        t1.Position = {-0.5f, 0.0f, -1.0f};
        t1.Scale = {1.0f, 1.0f, 1.0f};
        t1.Rotation = {0.0f, 0.0f, 0.0f};

        auto& mesh1 = m_Registry.AddMesh(e1);
        mesh1.VAO = std::make_unique<VertexArray>();
        mesh1.VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
        mesh1.EBO = std::make_unique<IndexBuffer>(indices, 36);

        BufferLayout layout;
        layout.PushFloat(3);

        mesh1.VAO->AddBuffer(*mesh1.VBO, layout);
        mesh1.EBO->Bind();
        mesh1.IndexCount = 36;

        auto e2 = m_Registry.CreateEntity();
        auto& t2 = m_Registry.AddTransform(e2);
        t2.Position = {0.5f, 0.0f, -2.0f};
        t2.Scale = {1.0f, 1.0f, 1.0f};
        t2.Rotation = {0.0f, 0.0f, 0.0f};

        auto& mesh2 = m_Registry.AddMesh(e2);
        mesh2.VAO = std::make_unique<VertexArray>();
        mesh2.VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
        mesh2.EBO = std::make_unique<IndexBuffer>(indices, 36);

        mesh2.VAO->AddBuffer(*mesh2.VBO, layout);
        mesh2.EBO->Bind();
        mesh2.IndexCount = 36;

        m_CameraEntity = m_Registry.CreateEntity();

        auto& cam = m_Registry.AddCamera(m_CameraEntity, 45.0f, 1.0f, 0.1f, 100.0f);
        cam.CameraData.SetPosition({0.0f, 0.0f, 3.0f});

        m_Registry.AddTransform(m_CameraEntity);
    }

    void OnUpdate(float dt) override
    {
        float speed = 3.0f;

        auto& camComp = m_Registry.GetCameras().at(m_CameraEntity);
        auto& camera  = camComp.CameraData;

        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front = glm::normalize(front);

        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0,1,0)));

        glm::vec3 pos = camera.GetPosition();

        if (Input::IsKeyPressed(Key_W))
            pos += front * speed * dt;

        if (Input::IsKeyPressed(Key_S))
            pos -= front * speed * dt;

        if (Input::IsKeyPressed(Key_A))
            pos -= right * speed * dt;

        if (Input::IsKeyPressed(Key_D))
            pos += right * speed * dt;

        camera.SetPosition(pos);

        float dx = Input::GetMouseDeltaX();
        float dy = Input::GetMouseDeltaY();

        float sensitivity = 0.1f;

        m_Yaw   += dx * sensitivity;
        m_Pitch -= dy * sensitivity;

        if (m_Pitch > 89.0f)  m_Pitch = 89.0f;
        if (m_Pitch < -89.0f) m_Pitch = -89.0f;

        camera.SetRotation(m_Yaw, m_Pitch);

        camera.SetAspect(
            (float)m_Window->GetWidth() / (float)m_Window->GetHeight()
        );

        for (auto& [entity, transform] : m_Registry.GetTransforms())
        {
            transform.Rotation.y += 50.0f * dt;
            transform.Rotation.x += 20.0f * dt;
        }

        Renderer::Draw(m_Registry,
                       m_Window->GetWidth(),
                       m_Window->GetHeight());
    }

    void OnShutdown() override {
    }
};

Application* CreateApplication()
{
    return new SandboxApp();
}

}