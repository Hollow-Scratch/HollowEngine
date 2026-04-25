#pragma once

namespace Raycaster {

class Renderer {
public:
    static void Init();
    static void Clear(float r, float g, float b);
    static void Draw();
};

}