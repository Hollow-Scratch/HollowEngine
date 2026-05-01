#include "Raycaster.h"
#include "ECS/Registry.hpp"
#include "Core/Window.hpp"
#include <iostream>

namespace Hollow {

class SandboxApp : public Application {
private:
    Registry m_Registry;

    Entity m_Player; // 🔥 controlled triangle

protected:
    void OnInit() override {
        std::cout << "[Sandbox] Init\n";

        // Player (movable)
        m_Player = m_Registry.CreateEntity();
        m_Registry.AddTransform(m_Player).Position = {-0.5f, 0.0f, 0.0f};
        m_Registry.AddMesh(m_Player);

        // Static triangle
        auto e2 = m_Registry.CreateEntity();
        m_Registry.AddTransform(e2).Position = {0.5f, 0.0f, 0.0f};
        m_Registry.AddMesh(e2);
    }

    void OnUpdate(float dt) override
    {
        auto& transform = m_Registry.GetTransforms()[m_Player];

        float speed = 1.5f;

        if (Input::IsKeyPressed(Key_W))
            transform.Position.y += speed * dt;

        if (Input::IsKeyPressed(Key_S))
            transform.Position.y -= speed * dt;

        if (Input::IsKeyPressed(Key_A))
            transform.Position.x -= speed * dt;

        if (Input::IsKeyPressed(Key_D))
            transform.Position.x += speed * dt;

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