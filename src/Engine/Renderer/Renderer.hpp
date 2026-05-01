#pragma once

#include <cstdint>
#include <memory>

namespace Hollow {

class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Shader;
class Framebuffer;
class Registry;

class Renderer
{
public:
    static void Init();
    static void Shutdown();
    static void ConfigureInstancing(VertexArray& vao);

    static void Clear(float r, float g, float b);
    static void Resize(uint32_t width, uint32_t height);

    static void Draw(Registry& registry, float width, float height);
    static uint32_t GetFinalImage();

private:
    static std::unique_ptr<VertexArray> s_VAO;
    static std::unique_ptr<VertexBuffer> s_VBO;
    static std::unique_ptr<IndexBuffer> s_EBO;
    static std::unique_ptr<Shader> s_Shader;
    static std::unique_ptr<Framebuffer> s_Framebuffer;
};

}
