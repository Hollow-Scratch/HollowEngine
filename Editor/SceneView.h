#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>

class SceneView : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    SceneView(QWidget* parent = nullptr);
    ~SceneView();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    QTimer* m_Timer;

    void Init();
    void Update();
    void Render();
};