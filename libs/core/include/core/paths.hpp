#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace note::paths {

// The environment values path resolution depends on.
struct environment {
    std::optional<std::string> appdata;        // Windows
    std::optional<std::string> home;           // macOS, and Linux fallback
    std::optional<std::string> xdg_data_home;  // Linux
};

// Pure: maps an environment to the notes-file path. No getenv, no static.
std::filesystem::path resolve_notes_file(const environment& env);

// The process-global notes file: resolved once from the real environment, cached.
const std::filesystem::path& notes_file();

}  // namespace note::paths
