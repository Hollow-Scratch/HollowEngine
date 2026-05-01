file(GLOB_RECURSE ENGINE_SRC
    Core/*.cpp
    Renderer/*.cpp
    Platform/*.cpp
    Graphics/*.cpp
    ECS/*.cpp
)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/assimp)

add_library(Engine SHARED
    ${ENGINE_SRC}
)

add_library(glad STATIC
    ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    ${CMAKE_SOURCE_DIR}/external/glad/include
)

target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external/glad/include
    ${CMAKE_SOURCE_DIR}/external/stb
    ${CMAKE_SOURCE_DIR}/external/assimp/include
)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(Engine PUBLIC
    glfw
    glm::glm
    glad
    assimp
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