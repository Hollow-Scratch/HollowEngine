cmake_minimum_required(VERSION 3.20)

add_library(AxiomEngine SHARED
Core/Application.cpp
Core/Window.cpp
Core/Time.cpp
Platform/Window/GLFWWindow.cpp
)

target_include_directories(AxiomEngine PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/Core
)

target_link_libraries(AxiomEngine PRIVATE
glad
glfw
)

target_compile_definitions(AxiomEngine PRIVATE AXIOM_ENGINE_EXPORTS)
set_target_properties(AxiomEngine PROPERTIES POSITION_INDEPENDENT_CODE ON) # PIC

