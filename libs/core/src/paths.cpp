#include "core/paths.hpp"

#include <cstdlib>
#include <optional>

namespace {

std::optional<std::filesystem::path> env_path(const char* name) {
    if (const char* v = std::getenv(name); v && *v) return std::filesystem::path{v};
    return std::nullopt;
}

}  // namespace

namespace note::paths {

std::filesystem::path notes_file() {
    static const auto path = []() {
        std::filesystem::path base;
#if defined(_WIN32)
        if (auto p = env_path("APPDATA")) base = *p;
#elif defined(__APPLE__)
        if (auto p = env_path("HOME")) base = *p / "Library" / "Application Support";
#else
        if (auto p = env_path("XDG_DATA_HOME"))
            base = *p;
        else if (auto p = env_path("HOME"))
            base = *p / ".local" / "share";
#endif
        return base / "note" / "notes.txt";
    }();
    return path;
}

}  // namespace note::paths
