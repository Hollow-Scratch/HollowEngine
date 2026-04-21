cmake_minimum_required(VERSION 3.20)

project(AXIOMENGINE LANGUAGES C CXX)

function(axiom_option NAME DEFAULT DESCRIPTION)
option(${NAME} "${DESCRIPTION}" ${DEFAULT})
endfunction()

function(axiom_define_toggle TARGET NAME)
if(DEFINED ${NAME} AND ${NAME})
target_compile_definitions(${TARGET} PRIVATE ${NAME}=1)
else()
target_compile_definitions(${TARGET} PRIVATE ${NAME}=0)
endif()
endfunction()

if(MSYS OR MINGW)
set(AXIOM_PLATFORM "MSYS2")
elseif(WIN32)
set(AXIOM_PLATFORM "WINDOWS")
elseif(UNIX)
set(AXIOM_PLATFORM "LINUX")
else()
set(AXIOM_PLATFORM "UNKNOWN")
endif()

if(MSVC)
set(AXIOM_COMPILER "MSVC")
set(AXIOM_COMPILER_FAMILY "MSVC")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
if(WIN32)
set(AXIOM_COMPILER "CLANG-CL")
set(AXIOM_COMPILER_FAMILY "MSVC")
else()
set(AXIOM_COMPILER "CLANG")
set(AXIOM_COMPILER_FAMILY "CLANG")
endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
set(AXIOM_COMPILER "GCC")
set(AXIOM_COMPILER_FAMILY "GCC")
else()
message(FATAL_ERROR "Unsupported compiler")
endif()

axiom_option(AXIOM_ENABLE_ASSERTS ON "")
axiom_option(AXIOM_ENABLE_LOGGING ON "")

axiom_option(AXIOM_ENABLE_ASAN OFF "")
axiom_option(AXIOM_ENABLE_UBSAN OFF "")
axiom_option(AXIOM_ENABLE_TSAN OFF "")

axiom_option(AXIOM_ENABLE_TRACY OFF "")
axiom_option(AXIOM_ENABLE_LTO ON "")
axiom_option(AXIOM_WARNINGS_AS_ERRORS OFF "")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC
${PROJECT_SOURCE_DIR}/external/glad/include
)

add_subdirectory(Engine)
add_subdirectory(Editor)
add_subdirectory(Sandbox)

include(CheckIPOSupported)

function(axiom_configure_target TARGET)

if(NOT TARGET ${TARGET})
message(FATAL_ERROR "Target not found")
endif()

target_compile_features(${TARGET} PRIVATE cxx_std_23)
set_target_properties(${TARGET} PROPERTIES CXX_EXTENSIONS OFF)

set_target_properties(glad PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(AXIOM_COMPILER_FAMILY STREQUAL "MSVC")
target_compile_options(${TARGET} PRIVATE
    $<$<CONFIG:Debug>:/Zi /Od>
    $<$<CONFIG:Release>:/O2>
)
else()
target_compile_options(${TARGET} PRIVATE
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Release>:-O2>
    -fno-omit-frame-pointer
)
endif()

if(AXIOM_COMPILER_FAMILY STREQUAL "MSVC")
target_compile_options(${TARGET} PRIVATE /W4)
else()
target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(AXIOM_WARNINGS_AS_ERRORS)
if(AXIOM_COMPILER_FAMILY STREQUAL "MSVC")
target_compile_options(${TARGET} PRIVATE /WX)
else()
target_compile_options(${TARGET} PRIVATE -Werror)
endif()
endif()

if(AXIOM_ENABLE_ASAN OR AXIOM_ENABLE_UBSAN OR AXIOM_ENABLE_TSAN)

if(AXIOM_ENABLE_TSAN AND AXIOM_PLATFORM STREQUAL "WINDOWS")
message(FATAL_ERROR "TSAN is not supported on Windows")
endif()

if(AXIOM_COMPILER_FAMILY STREQUAL "GCC" OR AXIOM_COMPILER_FAMILY STREQUAL "CLANG")

set(SAN_FLAGS "")

if(AXIOM_ENABLE_ASAN)
list(APPEND SAN_FLAGS address)
endif()

if(AXIOM_ENABLE_UBSAN)
list(APPEND SAN_FLAGS undefined)
endif()

if(AXIOM_ENABLE_TSAN)
list(APPEND SAN_FLAGS thread)
endif()

list(JOIN SAN_FLAGS "," SAN_FLAGS_STR)

target_compile_options(${TARGET} PRIVATE -fsanitize=${SAN_FLAGS_STR})
target_link_options(${TARGET} PRIVATE -fsanitize=${SAN_FLAGS_STR})

endif()

if(AXIOM_COMPILER_FAMILY STREQUAL "MSVC")

if(AXIOM_ENABLE_UBSAN)
message(WARNING "UBSAN is not well supported on MSVC")
endif()

if(AXIOM_ENABLE_ASAN)
target_compile_options(${TARGET} PRIVATE /fsanitize=address /Zi)
target_link_options(${TARGET} PRIVATE /fsanitize=address)
target_compile_definitions(${TARGET} PRIVATE _DISABLE_VECTOR_ANNOTATION)
endif()

endif()

endif()

if(AXIOM_ENABLE_LTO)
check_ipo_supported(RESULT IPO_OK OUTPUT IPO_ERR)
if(IPO_OK)
set_property(TARGET ${TARGET} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
endif()

axiom_define_toggle(${TARGET} AXIOM_ENABLE_ASSERTS)
axiom_define_toggle(${TARGET} AXIOM_ENABLE_LOGGING)

target_compile_definitions(${TARGET} PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

if(AXIOM_ENABLE_TRACY)
target_compile_definitions(${TARGET} PRIVATE TRACY_ENABLE)
endif()

endfunction()

axiom_configure_target(Sandbox)
axiom_configure_target(AxiomEngine)
axiom_configure_target(Editor)

if(CMAKE_EXPORT_COMPILE_COMMANDS AND NOT WIN32)
execute_process(
COMMAND ${CMAKE_COMMAND} -E create_symlink
${CMAKE_BINARY_DIR}/compile_commands.json
${CMAKE_SOURCE_DIR}/compile_commands.json
)
endif()

message(STATUS "========== Axiom Build ==========")
message(STATUS "Platform: ${AXIOM_PLATFORM}")
message(STATUS "Compiler: ${AXIOM_COMPILER}")
message(STATUS "Family: ${AXIOM_COMPILER_FAMILY}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Generator: ${CMAKE_GENERATOR}")
message(STATUS "ASAN: ${AXIOM_ENABLE_ASAN}")
message(STATUS "UBSAN: ${AXIOM_ENABLE_UBSAN}")
message(STATUS "TSAN: ${AXIOM_ENABLE_TSAN}")
message(STATUS "LTO: ${AXIOM_ENABLE_LTO}")
message(STATUS "=================================")
