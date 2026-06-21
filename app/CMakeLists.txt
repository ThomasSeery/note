add_executable(note
    src/main.cpp)

target_link_libraries(note
    PRIVATE
        note::core
        note_warnings)

install(TARGETS note DESTINATION bin)
