#include "MainWindow.hpp"
#include "SceneView.hpp"

#include <QDockWidget>
#include <QListWidget>
#include <QTextEdit>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    SetupUI();
}

void MainWindow::SetupUI()
{
    setWindowTitle("Editor");

    // Scene View
    m_SceneView = new SceneView(this);
    setCentralWidget(m_SceneView);

    // Hierarchy
    QDockWidget* hierarchy = new QDockWidget("Hierarchy", this);
    QListWidget* list = new QListWidget();
    list->addItem("Cube");
    list->addItem("Light");
    hierarchy->setWidget(list);
    addDockWidget(Qt::LeftDockWidgetArea, hierarchy);

    // Inspector
    QDockWidget* inspector = new QDockWidget("Inspector", this);
    QTextEdit* text = new QTextEdit();
    text->setText("Select an object...");
    inspector->setWidget(text);
    addDockWidget(Qt::RightDockWidgetArea, inspector);
}