cmake_minimum_required(VERSION 3.20)

add_executable(Game
  main.cpp
  Core/Application.cpp
  Core/Window.cpp
  Core/Platform/Window/GLFWWindow.cpp
)

target_include_directories(Game PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Core
  ${PROJECT_SOURCE_DIR}/external/glfw/include
)

target_link_libraries(Game PRIVATE
  glad
  glfw
)
