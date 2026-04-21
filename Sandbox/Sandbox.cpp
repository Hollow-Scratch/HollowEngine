#include <Core/Application.hpp>

namespace Axiom
{
    class Sandbox : public Application
    {
    public:
        Sandbox() = default;
        ~Sandbox() = default;

    protected:
        void OnInit() override
        {
            // Called once at startup
        }

        void OnUpdate(float deltaTime) override
        {
            (void)deltaTime;
            // Called every frame
        }

        void OnShutdown() override
        {
            // Called when closing
        }
    };

    Application* CreateApplication()
    {
        return new Sandbox();
    }
}