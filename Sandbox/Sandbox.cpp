#include "Raycaster.h"
#include <iostream>

namespace Hollow {

class SandboxApp : public Application {
protected:
    void OnInit() override {
        std::cout << "[Sandbox] Init\n";

        
    }

    void OnUpdate(float dt) override {
        (void)dt;

        

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
