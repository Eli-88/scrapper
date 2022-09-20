#pragma once

#include <memory>
#include <string>
#include <vector>

namespace Core {
class IRegexSearch {
 public:
  virtual ~IRegexSearch() = default;
  virtual std::vector<const std::string> Search(const std::string& content) = 0;
};

std::unique_ptr<IRegexSearch> CreateRegexSearch(const std::string& pattern);
}  // namespace Core