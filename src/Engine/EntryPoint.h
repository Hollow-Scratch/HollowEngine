#pragma once

#include "Core/Application.hpp"

namespace Raycaster {
    extern Application* CreateApplication();
}

int main()
{
    auto app = Raycaster::CreateApplication();
    app->Run();
    delete app;
    return 0;
}