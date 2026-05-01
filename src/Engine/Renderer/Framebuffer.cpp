#include "Framebuffer.hpp"

#include <glad/glad.h>

#include <cstdlib>
#include <iostream>

namespace Hollow {

namespace {

thread_local GLint s_PreviousFramebuffer = 0;

}

Framebuffer::Framebuffer(uint32_t width, uint32_t height)
    : m_Width(width), m_Height(height)
{
    Invalidate();
}

Framebuffer::~Framebuffer()
{
    Release();
}

void Framebuffer::Resize(uint32_t width, uint32_t height)
{
    if (width == 0 || height == 0)
        return;

    if (m_Width == width && m_Height == height)
        return;

    m_Width = width;
    m_Height = height;
    Invalidate();
}

void Framebuffer::Bind() const
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &s_PreviousFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void Framebuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(s_PreviousFramebuffer));
}

uint32_t Framebuffer::GetColorAttachmentID() const
{
    return m_ColorAttachment;
}

void Framebuffer::Invalidate()
{
    Release();

    if (m_Width == 0 || m_Height == 0)
        return;

    GLint previousFramebuffer = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFramebuffer);

    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    glGenTextures(1, &m_ColorAttachment);
    glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        static_cast<GLsizei>(m_Width),
        static_cast<GLsizei>(m_Height),
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        nullptr
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

    glGenRenderbuffers(1, &m_DepthAttachment);
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment);
    glRenderbufferStorage(
        GL_RENDERBUFFER,
        GL_DEPTH24_STENCIL8,
        static_cast<GLsizei>(m_Width),
        static_cast<GLsizei>(m_Height)
    );
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER,
        m_DepthAttachment
    );

    const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "[Framebuffer] Incomplete framebuffer: " << status << "\n";
        std::abort();
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(previousFramebuffer));
}

void Framebuffer::Release()
{
    if (m_DepthAttachment != 0)
    {
        glDeleteRenderbuffers(1, &m_DepthAttachment);
        m_DepthAttachment = 0;
    }

    if (m_ColorAttachment != 0)
    {
        glDeleteTextures(1, &m_ColorAttachment);
        m_ColorAttachment = 0;
    }

    if (m_FBO != 0)
    {
        glDeleteFramebuffers(1, &m_FBO);
        m_FBO = 0;
    }
}

}
