#include <catch2/catch_test_macros.hpp>

#include <filesystem>

#include "core/paths.hpp"

namespace fs = std::filesystem;
using note::paths::environment;
using note::paths::resolve_notes_file;

#if defined(_WIN32)

TEST_CASE("resolves under APPDATA on Windows") {
    environment env{.appdata = "C:\\Users\\t\\AppData\\Roaming"};
    CHECK(resolve_notes_file(env) == fs::path{"C:\\Users\\t\\AppData\\Roaming"} / "note" / "notes.txt");
}

#elif defined(__APPLE__)

TEST_CASE("resolves under ~/Library/Application Support on macOS") {
    environment env{.home = "/Users/t"};
    CHECK(resolve_notes_file(env) ==
          fs::path{"/Users/t"} / "Library" / "Application Support" / "note" / "notes.txt");
}

#else

TEST_CASE("prefers XDG_DATA_HOME on Linux") {
    environment env{.home = "/home/t", .xdg_data_home = "/home/t/.xdg"};
    CHECK(resolve_notes_file(env) == fs::path{"/home/t/.xdg"} / "note" / "notes.txt");
}

TEST_CASE("falls back to ~/.local/share when XDG_DATA_HOME is unset") {
    environment env{.home = "/home/t"};
    CHECK(resolve_notes_file(env) == fs::path{"/home/t"} / ".local" / "share" / "note" / "notes.txt");
}

#endif
