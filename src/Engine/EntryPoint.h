#pragma once

#include "Core/Application.hpp"

namespace Hollow {
    extern Application* CreateApplication();
}

int main()
{
    auto app = Hollow::CreateApplication();
    app->Run();
    delete app;
    return 0;
}
