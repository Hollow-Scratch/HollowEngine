set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)

add_executable(Editor
    main.cpp
    MainWindow.cpp
    MainWindow.hpp
    SceneView.cpp
    SceneView.hpp
)

target_link_libraries(Editor
    Qt6::Widgets
    Qt6::OpenGLWidgets
)