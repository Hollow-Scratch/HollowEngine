cmake_minimum_required(VERSION 3.20)

add_library(AxiomEngine STATIC
Core/Application.cpp
Core/Window.cpp
Core/Time.cpp
Core/Platform/Window/GLFWWindow.cpp
)

target_include_directories(AxiomEngine PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/Core
)

target_link_libraries(AxiomEngine PRIVATE
glad
glfw
)
