#include "Core/Input.hpp"
#include "Raycaster.h"
#include <iostream>

namespace Raycaster {

class SandboxApp : public Application {
protected:
    void OnInit() override {
        std::cout << "[Sandbox] Init\n";
    }

    void OnUpdate(float dt) override {
        // Example update logic
        // (replace with raycasting / movement later)
        (void)dt;

        //std::cout << "HAHAH";

        if(Input::IsKeyPressed(Raycaster::Key_W)) {
            std::cout << "W is down\n";
        }
    }

    void OnShutdown() override {
        //std::cout << "[Sandbox] Shutdown\n";
    }
};


Application* CreateApplication()
{
    return new SandboxApp();
}

} 
