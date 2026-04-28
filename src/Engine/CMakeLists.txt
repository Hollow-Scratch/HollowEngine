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

# Engine
add_library(Engine SHARED ${ENGINE_SRC})

target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external/entt/include
    ${CMAKE_SOURCE_DIR}/external/bgfx/include
    ${CMAKE_SOURCE_DIR}/external/bx/include
    ${CMAKE_SOURCE_DIR}/external/bimg/include
)

target_compile_definitions(Engine PUBLIC
    GLFW_INCLUDE_NONE
)

# Dependencies
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# bgfx libraries
target_link_libraries(Engine PUBLIC
    glfw
    glm::glm
    bgfx
    bx
    bimg
)

target_compile_options(Engine PRIVATE
    -Wall -Wextra -Wpedantic
)
