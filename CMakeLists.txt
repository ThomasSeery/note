cmake_minimum_required(VERSION 3.28)

project(note
    VERSION 0.1.0
    DESCRIPTION "A C++ project"
    LANGUAGES CXX)

if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    include(CTest)
endif()

# Warning flags gated by compiler (MSVC rejects GNU-style options).
add_library(note_warnings INTERFACE)
target_compile_options(note_warnings INTERFACE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall;-Wextra;-Wpedantic;-Wshadow;-Wconversion;-Wsign-conversion>
    $<$<CXX_COMPILER_ID:MSVC>:/W4;/permissive->)

option(NOTE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
if(NOTE_WARNINGS_AS_ERRORS)
    target_compile_options(note_warnings INTERFACE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
        $<$<CXX_COMPILER_ID:MSVC>:/WX>)
endif()

# Opt-in sanitizers: GNU/Clang get ASan+UBSan, MSVC gets ASan (no UBSan).
option(NOTE_SANITIZE "Build with AddressSanitizer (and UBSan where available)" OFF)
add_library(note_sanitize INTERFACE)
if(NOTE_SANITIZE)
    target_compile_options(note_sanitize INTERFACE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-fsanitize=address,undefined;-fno-omit-frame-pointer>
        $<$<CXX_COMPILER_ID:MSVC>:/fsanitize=address>)
    target_link_options(note_sanitize INTERFACE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-fsanitize=address,undefined>)
    # MSVC ASan annotates STL containers; Catch2 isn't built with ASan, so the
    # two disagree and the link fails (LNK2038). Turn the annotations off to mix.
    target_compile_definitions(note_sanitize INTERFACE
        $<$<CXX_COMPILER_ID:MSVC>:_DISABLE_STRING_ANNOTATION=1;_DISABLE_VECTOR_ANNOTATION=1>)
endif()

add_subdirectory(libs/core)
add_subdirectory(app)

if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
