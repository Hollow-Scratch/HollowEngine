#pragma once

#include <memory>

namespace Hollow {

class VertexArray;
class VertexBuffer;
class Shader;

class Renderer
{
public:
    static void Init();
    static void Shutdown();

    static void Clear(float r, float g, float b);
    static void Draw();

private:
    static std::unique_ptr<VertexArray> s_VAO;
    static std::unique_ptr<VertexBuffer> s_VBO;
    static std::unique_ptr<Shader> s_Shader;
};

}