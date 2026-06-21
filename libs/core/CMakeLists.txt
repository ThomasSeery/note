add_library(core
    src/core.cpp
    src/paths.cpp)

target_include_directories(core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(core
    PRIVATE note_warnings)

add_library(note::core ALIAS core)
