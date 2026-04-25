add_executable(Sandbox
    Sandbox.cpp
)

target_link_libraries(Sandbox PRIVATE Engine)

target_include_directories(Sandbox PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/Engine
)
