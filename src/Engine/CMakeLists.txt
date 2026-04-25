file(GLOB_RECURSE ENGINE_SRC
    Core/*.cpp
    Renderer/*.cpp
    Platform/*.cpp
    Graphics/*.cpp
)

file(GLOB_RECURSE ENGINE_HEADERS
    Core/*.hpp
    Renderer/*.hpp
    Platform/*.hpp
    Graphics/*.hpp
)

# GLAD
add_library(glad ../../external/glad/src/gl.c)

target_include_directories(glad PUBLIC
    ../../external/glad/include
)

set_target_properties(glad PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# Engine
add_library(Engine SHARED ${ENGINE_SRC} ${ENGINE_HEADERS})

target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external/entt/src
)

# Dependencies
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(Engine PUBLIC
    glfw
    glad
    OpenGL::GL
    glm::glm
)

target_compile_options(Engine PRIVATE
    -Wall -Wextra -Wpedantic
)