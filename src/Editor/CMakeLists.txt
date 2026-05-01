find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)

add_executable(Editor
    main.cpp
    EditorViewportWidget.cpp
    EditorViewportWidget.hpp
)

target_link_libraries(Editor PRIVATE
    Engine
    Qt6::Widgets
    Qt6::OpenGLWidgets
    OpenGL::GL
)

target_include_directories(Editor PRIVATE
    ${CMAKE_SOURCE_DIR}/src/Engine
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external/glad/include
)

target_sources(Editor PRIVATE
    ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c
)