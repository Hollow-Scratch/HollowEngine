cmake_minimum_required(VERSION 3.20)

project(AXIOMENGINE LANGUAGES C CXX)

# Options
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

# Core toggles
axiom_option(AXIOM_ENABLE_ASSERTS ON  "Enable engine assertions")
axiom_option(AXIOM_ENABLE_LOGGING ON  "Enable engine logging")

# Sanitizer defaults
set(AXIOM_DEFAULT_ASAN ON)
set(AXIOM_DEFAULT_UBSAN ON)
set(AXIOM_DEFAULT_TSAN OFF)

# Disable sanitizer defaults on Windows
# clang-cl on Windows is currently problematic in this setup,
# and GCC on Windows is also disabled.
if(WIN32)
    set(AXIOM_DEFAULT_ASAN OFF)
    set(AXIOM_DEFAULT_UBSAN OFF)
    set(AXIOM_DEFAULT_TSAN OFF)
endif()

# Sanitizers
axiom_option(AXIOM_ENABLE_ASAN  ${AXIOM_DEFAULT_ASAN}  "Enable AddressSanitizer")
axiom_option(AXIOM_ENABLE_UBSAN ${AXIOM_DEFAULT_UBSAN} "Enable UndefinedBehaviorSanitizer")
axiom_option(AXIOM_ENABLE_TSAN  ${AXIOM_DEFAULT_TSAN}  "Enable ThreadSanitizer")

# Tooling
axiom_option(AXIOM_ENABLE_TRACY       OFF "Enable Tracy profiler")
axiom_option(AXIOM_ENABLE_LTO         ON "Enable link-time optimization")
axiom_option(AXIOM_WARNINGS_AS_ERRORS ON  "Treat warnings as errors")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler detection
if(MSVC)
    set(AXIOM_COMPILER "MSVC")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(AXIOM_COMPILER "GCC")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(AXIOM_COMPILER "CLANG")
else()
    message(FATAL_ERROR "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

# Dependencies
message(STATUS "---- Dependencies ----")

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

#add_subdirectory(external/glad/cmake)
#glad_add_library(glad REPRODUCIBLE API gl:core=4.6)

#set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
#set(ASSIMP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
#add_subdirectory(external/assimp)

#add_subdirectory(external/spdlog)

#if(AXIOM_ENABLE_TRACY)
#    add_subdirectory(external/tracy)
#endif()

# Engine
add_subdirectory(src)

# Target configuration
include(CheckIPOSupported)

function(axiom_configure_target TARGET)
    if(NOT TARGET ${TARGET})
        message(FATAL_ERROR "axiom_configure_target: target '${TARGET}' does not exist")
    endif()

    # C++ standard per target
    target_compile_features(${TARGET} PRIVATE cxx_std_23)
    set_target_properties(${TARGET} PROPERTIES
        CXX_EXTENSIONS OFF
    )

    # Debug info + optimization per compiler/config
    if(MSVC)
        target_compile_options(${TARGET} PRIVATE
            $<$<CONFIG:Debug>:/Zi /Od>
            $<$<CONFIG:Release>:/O2>
            $<$<CONFIG:RelWithDebInfo>:/Zi /O2>
            $<$<CONFIG:MinSizeRel>:/O1>
        )
    else()
        target_compile_options(${TARGET} PRIVATE
            $<$<CONFIG:Debug>:-g -O0>
            $<$<CONFIG:Release>:-O2>
            $<$<CONFIG:RelWithDebInfo>:-g -O2>
            $<$<CONFIG:MinSizeRel>:-Os>
        )

        # Better profiler/debugger stack traces
        target_compile_options(${TARGET} PRIVATE -fno-omit-frame-pointer)
    endif()

    # Warnings
    include(${CMAKE_SOURCE_DIR}/cmake/compiler_warnings.cmake)

    if(AXIOM_WARNINGS_AS_ERRORS)
        if(MSVC)
            target_compile_options(${TARGET} PRIVATE /WX)
        else()
            target_compile_options(${TARGET} PRIVATE -Werror)
        endif()
    endif()

    # Sanitizers
    if(AXIOM_ENABLE_ASAN OR AXIOM_ENABLE_UBSAN OR AXIOM_ENABLE_TSAN)
        if(AXIOM_ENABLE_ASAN AND AXIOM_ENABLE_TSAN)
            message(FATAL_ERROR "ASAN and TSAN cannot be enabled together")
        endif()

        set(AXIOM_SANITIZERS_SUPPORTED FALSE)

        # Supported cases:
        # - Clang on non-Windows platforms
        # - GCC on non-Windows platforms
        if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND NOT WIN32)
            set(AXIOM_SANITIZERS_SUPPORTED TRUE)
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT WIN32)
            set(AXIOM_SANITIZERS_SUPPORTED TRUE)
        endif()

        if(AXIOM_SANITIZERS_SUPPORTED)
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

            if(SAN_FLAGS)
                list(JOIN SAN_FLAGS "," SAN_FLAGS_STR)
                target_compile_options(${TARGET} PRIVATE -fsanitize=${SAN_FLAGS_STR})
                target_link_options(${TARGET} PRIVATE -fsanitize=${SAN_FLAGS_STR})
            endif()
        else()
            message(STATUS
                "Sanitizers requested for target '${TARGET}', but they are disabled for "
                "${CMAKE_CXX_COMPILER_ID} on ${CMAKE_SYSTEM_NAME}."
            )
        endif()
    endif()

    # LTO / IPO
    if(AXIOM_ENABLE_LTO)
        check_ipo_supported(RESULT AXIOM_IPO_SUPPORTED OUTPUT AXIOM_IPO_ERROR)
        if(AXIOM_IPO_SUPPORTED)
            set_property(TARGET ${TARGET} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
        else()
            message(WARNING "IPO/LTO not supported for target '${TARGET}': ${AXIOM_IPO_ERROR}")
        endif()
    endif()

    # Feature toggles
    axiom_define_toggle(${TARGET} AXIOM_ENABLE_ASSERTS)
    axiom_define_toggle(${TARGET} AXIOM_ENABLE_LOGGING)

    # Standard debug/release defines
    target_compile_definitions(${TARGET} PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:MinSizeRel>>:NDEBUG>
    )

    # Tracy
    if(AXIOM_ENABLE_TRACY)
        target_compile_definitions(${TARGET} PRIVATE TRACY_ENABLE)
        target_link_libraries(${TARGET} PRIVATE Tracy::TracyClient)
    endif()
endfunction()

# Apply target config
axiom_configure_target(Game)

# If you also have Sandbox / Editor / Tests, configure them too:
# axiom_configure_target(Sandbox)
# axiom_configure_target(Editor)
# axiom_configure_target(UnitTests)

# compile_commands convenience
if(CMAKE_EXPORT_COMPILE_COMMANDS AND NOT WIN32)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
                ${CMAKE_BINARY_DIR}/compile_commands.json
                ${CMAKE_SOURCE_DIR}/compile_commands.json
        RESULT_VARIABLE AXIOM_SYMLINK_RESULT
        ERROR_QUIET
    )
endif()

# Info dump
message(STATUS "========== Axiom Build ==========")
message(STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiler: ${AXIOM_COMPILER}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Generator: ${CMAKE_GENERATOR}")

if(CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Configurations: ${CMAKE_CONFIGURATION_TYPES}")
else()
    message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
endif()

message(STATUS "ASAN: ${AXIOM_ENABLE_ASAN}")
message(STATUS "UBSAN: ${AXIOM_ENABLE_UBSAN}")
message(STATUS "TSAN: ${AXIOM_ENABLE_TSAN}")
message(STATUS "TRACY: ${AXIOM_ENABLE_TRACY}")
message(STATUS "LTO: ${AXIOM_ENABLE_LTO}")
message(STATUS "Warnings as Errors: ${AXIOM_WARNINGS_AS_ERRORS}")
message(STATUS "=================================")
