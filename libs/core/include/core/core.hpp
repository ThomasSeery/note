#pragma once

#include <expected>
#include <string>
#include <vector>

namespace note {

std::expected<void, std::string> add(const std::string& note);

std::expected<std::vector<std::string>, std::string> load();

}  // namespace note
