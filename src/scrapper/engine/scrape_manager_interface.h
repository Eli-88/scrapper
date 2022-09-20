#pragma once

#include <string>
#include <vector>

namespace Scrapper {
class IScrapeManager {
 public:
  using RegexCollection = std::vector<std::string>;
  virtual ~IScrapeManager() = default;
  virtual void Run() = 0;

  virtual void Dispatch(const std::string& host,
                        const std::string& port,
                        const std::string& target,
                        int version,
                        const RegexCollection& regexCollection) = 0;
};
}  // namespace Scrapper