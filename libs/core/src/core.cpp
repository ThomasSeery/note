#include "core/core.hpp"
#include "core/paths.hpp"

#include <fstream>
#include <ios>

namespace note {

std::expected<void, std::string> add(const std::string& note) {
    const auto path = paths::notes_file();
    std::ofstream f{path, std::ios::app};
    f << note << '\n';
    if (f) return {};
    return std::unexpected("Something went wrong");
}

std::expected<std::vector<std::string>, std::string> load() {
    std::vector<std::string> notes{};
    const auto path = paths::notes_file();
    std::ifstream f{path};
    std::string line;
    while (std::getline(f, line)) notes.emplace_back(std::move(line));
    return notes;
}

}  // namespace note
