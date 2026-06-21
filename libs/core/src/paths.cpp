#include "core/paths.hpp"

#include <cstdlib>

namespace note::paths {

std::filesystem::path resolve_notes_file(const environment& env) {
    std::filesystem::path base;
#if defined(_WIN32)
    if (env.appdata) base = *env.appdata;
#elif defined(__APPLE__)
    if (env.home) base = std::filesystem::path{*env.home} / "Library" / "Application Support";
#else
    if (env.xdg_data_home)
        base = *env.xdg_data_home;
    else if (env.home)
        base = std::filesystem::path{*env.home} / ".local" / "share";
#endif
    return base / "note" / "notes.txt";
}

const std::filesystem::path& notes_file() {
    static const std::filesystem::path path = [] {
        environment env;
        if (const char* v = std::getenv("APPDATA"); v && *v) env.appdata = v;
        if (const char* v = std::getenv("HOME"); v && *v) env.home = v;
        if (const char* v = std::getenv("XDG_DATA_HOME"); v && *v) env.xdg_data_home = v;
        return resolve_notes_file(env);
    }();
    return path;
}

}  // namespace note::paths
