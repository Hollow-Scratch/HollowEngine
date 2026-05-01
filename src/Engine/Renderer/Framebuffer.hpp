#pragma once

#include <cstdint>

namespace Hollow {

class Framebuffer
{
public:
    Framebuffer(uint32_t width, uint32_t height);
    ~Framebuffer();

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;

    void Resize(uint32_t width, uint32_t height);

    void Bind() const;
    void Unbind() const;

    uint32_t GetColorAttachmentID() const;

private:
    void Invalidate();
    void Release();

private:
    uint32_t m_FBO = 0;
    uint32_t m_ColorAttachment = 0;
    uint32_t m_DepthAttachment = 0;
    uint32_t m_Width = 0;
    uint32_t m_Height = 0;
};

}
