#include <glad/glad.h>

#include "EditorViewportWidget.hpp"

#include "Renderer/Renderer.hpp"

#include <QOpenGLContext>
#include <QPaintEvent>

#include <cstdlib>

namespace Hollow {

namespace {

void* ResolveOpenGLSymbol(const char* name)
{
    QOpenGLContext* context = QOpenGLContext::currentContext();
    if (!context)
        return nullptr;

    return reinterpret_cast<void*>(context->getProcAddress(name));
}

constexpr float kFullscreenTriangle[] = {
    -1.0f, -1.0f, 0.0f, 0.0f,
     3.0f, -1.0f, 2.0f, 0.0f,
    -1.0f,  3.0f, 0.0f, 2.0f
};

constexpr const char* kPresentVertexShader = R"(
#version 330 core
layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_UV;

out vec2 v_UV;

void main()
{
    v_UV = a_UV;
    gl_Position = vec4(a_Position, 0.0, 1.0);
}
)";

constexpr const char* kPresentFragmentShader = R"(
#version 330 core
in vec2 v_UV;

out vec4 FragColor;

uniform sampler2D u_ViewportTexture;

void main()
{
    FragColor = texture(u_ViewportTexture, v_UV);
}
)";

}

EditorViewportWidget::EditorViewportWidget(Registry& registry, QWidget* parent)
    : QOpenGLWidget(parent),
      m_Registry(registry)
{
    connect(&m_RepaintTimer, &QTimer::timeout, this, QOverload<>::of(&EditorViewportWidget::update));
    m_RepaintTimer.start(16);
}

EditorViewportWidget::~EditorViewportWidget()
{
    makeCurrent();
    DestroyFullscreenResources();
    Renderer::Shutdown();
    doneCurrent();
}

void EditorViewportWidget::initializeGL()
{
    if (!gladLoadGLLoader(ResolveOpenGLSymbol))
        std::abort();

    initializeOpenGLFunctions();

    Renderer::Init();
    CreateFullscreenResources();

    m_PresentProgram.addShaderFromSourceCode(QOpenGLShader::Vertex, kPresentVertexShader);
    m_PresentProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, kPresentFragmentShader);
    m_PresentProgram.link();
}

void EditorViewportWidget::resizeGL(int width, int height)
{
    Renderer::Resize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
}

void EditorViewportWidget::paintGL()
{
    Renderer::Draw(m_Registry, static_cast<float>(width()), static_cast<float>(height()));

    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
    glViewport(0, 0, width(), height());
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_PresentProgram.bind();
    m_PresentProgram.setUniformValue("u_ViewportTexture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Renderer::GetFinalImage());

    glBindVertexArray(m_QuadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    m_PresentProgram.release();

    glEnable(GL_DEPTH_TEST);
}

void EditorViewportWidget::paintEvent(QPaintEvent* event)
{
    QOpenGLWidget::paintEvent(event);
}

void EditorViewportWidget::CreateFullscreenResources()
{
    glGenVertexArrays(1, &m_QuadVAO);
    glGenBuffers(1, &m_QuadVBO);

    glBindVertexArray(m_QuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kFullscreenTriangle), kFullscreenTriangle, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(float) * 4,
        reinterpret_cast<void*>(sizeof(float) * 2)
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void EditorViewportWidget::DestroyFullscreenResources()
{
    if (m_QuadVBO != 0)
    {
        glDeleteBuffers(1, &m_QuadVBO);
        m_QuadVBO = 0;
    }

    if (m_QuadVAO != 0)
    {
        glDeleteVertexArrays(1, &m_QuadVAO);
        m_QuadVAO = 0;
    }
}

}
