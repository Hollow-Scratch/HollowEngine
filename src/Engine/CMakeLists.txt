file(GLOB_RECURSE ENGINE_SRC
    Core/*.cpp
    Renderer/*.cpp
    Platform/*.cpp
)

file(GLOB_RECURSE ENGINE_HEADERS
    Core/*.hpp
    Renderer/*.hpp
    Platform/*.hpp
)

add_library(glad ../../external/glad/src/gl.c)

target_include_directories(glad PUBLIC
    ../../external/glad/include
)

# Engine
add_library(Engine SHARED ${ENGINE_SRC} ${ENGINE_HEADERS})

target_include_directories(Engine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(glfw3 REQUIRED)

target_link_libraries(Engine
    PUBLIC glfw glad GL
)

target_compile_options(Engine PRIVATE
    -Wall -Wextra -Wpedantic
)

set_target_properties(glad PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)