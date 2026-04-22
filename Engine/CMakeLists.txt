cmake_minimum_required(VERSION 3.20)

add_library(AxiomEngine SHARED
    Core/Application.cpp
    Core/Window.cpp
    Core/Time.cpp
)

# Platform backends
if(WIN32)
    target_sources(AxiomEngine PRIVATE
        Platform/Window/Win32Window.cpp
    )
endif()

if(UNIX AND NOT APPLE)
    target_sources(AxiomEngine PRIVATE
        Platform/Window/X11Window.cpp
    )
endif()

# Includes
target_include_directories(AxiomEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Core
)

# X11 (Linux)
if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    target_link_libraries(AxiomEngine PRIVATE X11)
endif()

# Core dependencies (keep glad if you're using OpenGL later)
target_link_libraries(AxiomEngine PRIVATE
    glad
)

# Definitions
target_compile_definitions(AxiomEngine PRIVATE AXIOM_ENGINE_EXPORTS)

set_target_properties(AxiomEngine PROPERTIES POSITION_INDEPENDENT_CODE ON)