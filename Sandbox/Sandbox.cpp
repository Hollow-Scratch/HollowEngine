#include "Raycaster.h"
#include "ECS/Registry.hpp"
#include "Core/Window.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/BufferLayout.hpp"
#include "Graphics/Texture.hpp"

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
            -0.5f,-0.5f, 0.5f,  0.0f,0.0f,
             0.5f,-0.5f, 0.5f,  1.0f,0.0f,
             0.5f, 0.5f, 0.5f,  1.0f,1.0f,
            -0.5f, 0.5f, 0.5f,  0.0f,1.0f,

             0.5f,-0.5f,-0.5f,  0.0f,0.0f,
            -0.5f,-0.5f,-0.5f,  1.0f,0.0f,
            -0.5f, 0.5f,-0.5f,  1.0f,1.0f,
             0.5f, 0.5f,-0.5f,  0.0f,1.0f,

            -0.5f,-0.5f,-0.5f,  0.0f,0.0f,
            -0.5f,-0.5f, 0.5f,  1.0f,0.0f,
            -0.5f, 0.5f, 0.5f,  1.0f,1.0f,
            -0.5f, 0.5f,-0.5f,  0.0f,1.0f,

             0.5f,-0.5f, 0.5f,  0.0f,0.0f,
             0.5f,-0.5f,-0.5f,  1.0f,0.0f,
             0.5f, 0.5f,-0.5f,  1.0f,1.0f,
             0.5f, 0.5f, 0.5f,  0.0f,1.0f,

            -0.5f, 0.5f, 0.5f,  0.0f,0.0f,
             0.5f, 0.5f, 0.5f,  1.0f,0.0f,
             0.5f, 0.5f,-0.5f,  1.0f,1.0f,
            -0.5f, 0.5f,-0.5f,  0.0f,1.0f,

            -0.5f,-0.5f,-0.5f,  0.0f,0.0f,
             0.5f,-0.5f,-0.5f,  1.0f,0.0f,
             0.5f,-0.5f, 0.5f,  1.0f,1.0f,
            -0.5f,-0.5f, 0.5f,  0.0f,1.0f
        };

        unsigned int indices[] = {
            0,1,2, 2,3,0,
            4,5,6, 6,7,4,
            8,9,10, 10,11,8,
            12,13,14, 14,15,12,
            16,17,18, 18,19,16,
            20,21,22, 22,23,20
        };

        BufferLayout layout;
        layout.PushFloat(3);
        layout.PushFloat(2);

        auto sharedVAO = std::make_shared<VertexArray>();
        auto sharedVBO = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
        auto sharedEBO = std::make_shared<IndexBuffer>(indices, 36);
        auto sharedTexture = std::make_shared<Texture>("assets/textures/crate.png");

        sharedVAO->AddBuffer(*sharedVBO, layout);
        sharedEBO->Bind();

        int count = 32;

        for (int x = 0; x < count; x++)
        for (int y = 0; y < count; y++)
        for (int z = 0; z < count; z++)
        {
            Entity e = m_Registry.CreateEntity();

            auto& t = m_Registry.AddTransform(e);
            t.Position = {
                (x - count / 2) * 2.0f,
                (y - count / 2) * 2.0f,
                (z - count / 2) * 2.0f
            };
            m_Registry.AddAABB(e);

            auto& mesh = m_Registry.AddMesh(e);
            mesh.VAO = sharedVAO;
            mesh.VBO = sharedVBO;
            mesh.EBO = sharedEBO;
            mesh.TextureData = sharedTexture;
            mesh.IndexCount = 36;
        }

        m_CameraEntity = m_Registry.CreateEntity();

        auto& cam = m_Registry.AddCamera(m_CameraEntity, 45.0f, 1.0f, 0.1f, 100.0f);
        cam.CameraData.SetPosition({0.0f, 0.0f, 10.0f});

        m_Registry.AddTransform(m_CameraEntity);
    }

    void OnUpdate(float dt) override
    {
        float speed = 5.0f;

        auto& camComp = m_Registry.GetCameras().at(m_CameraEntity);
        auto& camera  = camComp.CameraData;

        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front = glm::normalize(front);

        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0,1,0)));

        glm::vec3 pos = camera.GetPosition();

        if (Input::IsKeyPressed(Key_W)) pos += front * speed * dt;
        if (Input::IsKeyPressed(Key_S)) pos -= front * speed * dt;
        if (Input::IsKeyPressed(Key_A)) pos -= right * speed * dt;
        if (Input::IsKeyPressed(Key_D)) pos += right * speed * dt;

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

        Renderer::Draw(
            m_Registry,
            m_Window->GetWidth(),
            m_Window->GetHeight()
        );
    }

    void OnShutdown() override {}
};

Application* CreateApplication()
{
    return new SandboxApp();
}

}
