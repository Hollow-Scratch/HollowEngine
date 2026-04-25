#include "Raycaster.h"
#include <iostream>

namespace Raycaster {

class SandboxApp : public Application {
protected:
    void OnInit() override {
        std::cout << "[Sandbox] Init\n";

        Renderer::Init();
    }

    void OnUpdate(float dt) override {
        (void)dt;

        Renderer::Clear(0.1f, 0.1f, 0.2f);

        Renderer::Draw();

        if (Input::IsKeyPressed(Key_W)) {
            std::cout << "W is down\n";
        }
    }

    void OnShutdown() override {
    }
};

Application* CreateApplication()
{
    return new SandboxApp();
}

}