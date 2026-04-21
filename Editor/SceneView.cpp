#include "SceneView.hpp"

SceneView::SceneView(QWidget* parent)
    : QOpenGLWidget(parent)
{
    m_Timer = new QTimer(this);

    // 60 FPS update loop
    connect(m_Timer, &QTimer::timeout, this, QOverload<>::of(&SceneView::update));
    m_Timer->start(16);
}

SceneView::~SceneView()
{
}

void SceneView::initializeGL()
{
    // Initialize Qt OpenGL functions
    initializeOpenGLFunctions();

    Init();
}

void SceneView::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void SceneView::paintGL()
{
    Update();
    Render();
}

void SceneView::Init()
{
    glEnable(GL_DEPTH_TEST);
}

void SceneView::Update()
{
    // Update logic (camera, scene, etc.)
}

void SceneView::Render()
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}