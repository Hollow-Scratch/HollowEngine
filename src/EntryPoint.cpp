#include "Core/Application.hpp"

extern Axiom::Application* Axiom::CreateApplication();

int main()
{
    auto app = Axiom::CreateApplication();
    app->run();
    delete app;
}