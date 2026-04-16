#pragma once

#include <QMainWindow>

class SceneView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private:
    SceneView* m_SceneView;
    void SetupUI();
};