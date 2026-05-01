#include "EditorViewportWidget.hpp"

#include "ECS/Registry.hpp"

#include <QApplication>
#include <QMainWindow>
#include <QSurfaceFormat>

int main(int argc, char* argv[])
{
    QSurfaceFormat format;
    format.setVersion(4, 6);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);

    Hollow::Registry registry;

    const Hollow::Entity cameraEntity = registry.CreateEntity();
    auto& camera = registry.AddCamera(cameraEntity, 45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    camera.CameraData.SetPosition({0.0f, 0.0f, 5.0f});
    registry.AddTransform(cameraEntity);

    QMainWindow window;
    auto* viewport = new Hollow::EditorViewportWidget(registry);
    window.setCentralWidget(viewport);
    window.resize(1280, 720);
    window.setWindowTitle("Axiom Editor");
    window.show();

    return app.exec();
}
