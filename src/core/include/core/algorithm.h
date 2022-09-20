#pragma once

#include <string>
#include <vector>

namespace Core {
std::vector<std::string> SplitString(const std::string& target,
                                     const std::string& delimiter);

}  // namespace Core