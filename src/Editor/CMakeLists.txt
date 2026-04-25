add_executable(Editor
    main.cpp
)

target_link_libraries(Editor PRIVATE Engine)

target_include_directories(Editor PRIVATE
    ${CMAKE_SOURCE_DIR}/src/Engine
)