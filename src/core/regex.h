#pragma once

#include "core/interface/regex_interface.h"

#include <boost/regex.hpp>

namespace Core {
class RegexSearch : public IRegexSearch {
 public:
  RegexSearch(const std::string& pattern);
  virtual std::vector<const std::string> Search(
      const std::string& content) override;

 private:
  boost::regex expression_;
};
}  // namespace Core
