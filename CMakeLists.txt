cmake_minimum_required(VERSION 3.16)
project(RayCastingEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(BUILD_MODE "Release" CACHE STRING "Build mode: Debug, Release, RelWithDebInfo")
set_property(CACHE BUILD_MODE PROPERTY STRINGS Debug Release RelWithDebInfo)

set(CMAKE_BUILD_TYPE ${BUILD_MODE} CACHE STRING "" FORCE)

option(HEAVY_RELEASE "Enable aggressive optimization flags" OFF)

set(CMAKE_CXX_FLAGS_DEBUG "-O1 -g -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS_DEBUG   "-O1 -g -fno-omit-frame-pointer")

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE   "-O3 -DNDEBUG")

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O2 -g")

if(HEAVY_RELEASE AND CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Heavy Release Enabled")

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -flto")
    set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} -march=native -flto")

    # optional
    # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffast-math")
endif()

add_subdirectory(src/Engine)
add_subdirectory(Sandbox)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            ${CMAKE_BINARY_DIR}/bin/assets
)

message(STATUS "Build Mode: ${CMAKE_BUILD_TYPE}")
message(STATUS "CXX Flags: ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}")