#include <filesystem>
#include <print>
#include <ranges>
#include <span>

#include "core/core.hpp"
#include "core/paths.hpp"

namespace {
constexpr std::string_view cmd_add = "add";
constexpr std::string_view cmd_ls = "ls";
}  // namespace

int main(int argc, char** argv) {
    const auto args = std::span{argv, static_cast<std::size_t>(argc)};
    if (args.size() < 2) {
        std::println("You need more argments.");
        return 0;
    }

    std::filesystem::create_directories(note::paths::notes_file().parent_path());

    const std::string_view cmd = args[1];
    if (cmd == cmd_add) {
        if (auto result = note::add(args[2]))
            std::println("Added \"{}\" to notes.", args[2]);
        else
            std::println("Error: {}", result.error());
    } else if (cmd == cmd_ls) {
        if (auto result = note::load()) {
            for (auto [i, note] : std::views::enumerate(*result))
                std::println("{}. {}", i + 1, note);
        } else
            std::println("Error: {}", result.error());
    }
    return 0;
}
