cmake_minimum_required(VERSION 3.20)

add_executable(Game
  main.cpp
  Core/Application.cpp
  Core/Window.cpp
  Core/Platform/Window/Win32Window.cpp
)

target_include_directories(Game PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Core
)