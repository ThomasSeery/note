add_executable(note
    src/main.cpp)

target_link_libraries(note
    PRIVATE
        note::core
        note_warnings
        note_sanitize)

# Install into a directory already on PATH so `note` runs by bare name from
# anywhere — no PATH edits, no `.\` prefix. The directory differs per OS.
if(WIN32)
    set(note_install_default "$ENV{LOCALAPPDATA}/Microsoft/WindowsApps")
else()
    set(note_install_default "$ENV{HOME}/.local/bin")
endif()

# Use that as the install prefix, unless the user passed their own --prefix.
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${note_install_default}" CACHE PATH "" FORCE)
endif()

# "." installs the exe directly into the prefix dir (the on-PATH dir itself, not a <prefix>/bin).
install(TARGETS note RUNTIME DESTINATION ".")
