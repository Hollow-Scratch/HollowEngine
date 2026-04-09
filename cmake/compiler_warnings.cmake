include_guard(GLOBAL)

function(axiom_set_compiler_warnings TARGET)
    if(NOT TARGET "${TARGET}")
        message(FATAL_ERROR "axiom_set_compiler_warnings: target '${TARGET}' does not exist")
    endif()

    set(options WARNINGS_AS_ERRORS)
    cmake_parse_arguments(AXIOM_WARN "${options}" "" "" ${ARGN})

    if(MSVC)
        target_compile_options("${TARGET}" PRIVATE
            /W4
            /permissive-
            /w14242
            /w14254
            /w14263
            /w14265
            /w14287
            /we4289
            /w14296
            /w14311
            /w14545
            /w14546
            /w14547
            /w14549
            /w14555
            /w14619
            /w14640
            /w14826
            /w14905
            /w14906
            /w14928
        )

        if(AXIOM_WARN_WARNINGS_AS_ERRORS)
            target_compile_options("${TARGET}" PRIVATE /WX)
        endif()

    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
        target_compile_options("${TARGET}" PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wcast-align
            -Wunused
            -Woverloaded-virtual
            -Wnull-dereference
            -Wdouble-promotion
            -Wformat=2
        )

        if(AXIOM_WARN_WARNINGS_AS_ERRORS)
            target_compile_options("${TARGET}" PRIVATE -Werror)
        endif()

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options("${TARGET}" PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wcast-align
            -Wunused
            -Woverloaded-virtual
            -Wnull-dereference
            -Wdouble-promotion
            -Wformat=2
        )

        if(AXIOM_WARN_WARNINGS_AS_ERRORS)
            target_compile_options("${TARGET}" PRIVATE -Werror)
        endif()

    else()
        message(WARNING
            "axiom_set_compiler_warnings: unknown compiler "
            "'${CMAKE_CXX_COMPILER_ID}' for target '${TARGET}'. "
            "No warning flags were applied."
        )
    endif()
endfunction()