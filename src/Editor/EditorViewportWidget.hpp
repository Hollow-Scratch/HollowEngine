#pragma once

#include <cstdint>

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QTimer>

#include "ECS/Registry.hpp"

class QPaintEvent;

namespace Hollow {

class EditorViewportWidget final
    : public QOpenGLWidget,
      protected QOpenGLFunctions_3_3_Core
{
public:
    explicit EditorViewportWidget(Registry& registry, QWidget* parent = nullptr);
    ~EditorViewportWidget() override;

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;
    void paintEvent(QPaintEvent* event) override;

private:
    void CreateFullscreenResources();
    void DestroyFullscreenResources();

private:
    Registry& m_Registry;
    QOpenGLShaderProgram m_PresentProgram;
    QTimer m_RepaintTimer;
    uint32_t m_QuadVAO = 0;
    uint32_t m_QuadVBO = 0;
};

}
