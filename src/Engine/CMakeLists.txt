# Source files
file(GLOB_RECURSE ENGINE_SRC
    Core/*.cpp
    Renderer/*.cpp
    Platform/*.cpp
    Graphics/*.cpp
)

# Engine library
add_library(Engine SHARED
    ${ENGINE_SRC}
)

# GLAD (separate library = clean)
add_library(glad STATIC
    ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    ${CMAKE_SOURCE_DIR}/external/glad/include
)

# Includes
target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external/glad/include
)

# Dependencies
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Link everything
target_link_libraries(Engine PUBLIC
    glfw
    glm::glm
    glad
)

target_compile_definitions(Engine PUBLIC
    GLFW_INCLUDE_NONE
)

target_compile_options(Engine PRIVATE
    -Wall -Wextra -Wpedantic
)

set_target_properties(glad PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)