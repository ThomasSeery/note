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

add_library(note_warnings INTERFACE)
target_compile_options(note_warnings INTERFACE
    -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)

option(NOTE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
if(NOTE_WARNINGS_AS_ERRORS)
    target_compile_options(note_warnings INTERFACE -Werror)
endif()

add_subdirectory(libs/core)
add_subdirectory(app)

if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
