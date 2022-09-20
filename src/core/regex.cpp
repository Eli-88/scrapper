#include "regex.h"
#include <boost/regex.hpp>

namespace Core {

RegexSearch::RegexSearch(const std::string& pattern) : expression_(pattern) {}

std::vector<const std::string> RegexSearch::Search(const std::string& content) {
  boost::match_results<std::string::const_iterator> match;
  std::vector<const std::string> result;
  if (boost::regex_search(content.cbegin(), content.cend(), match,
                          expression_)) {
    return std::vector<const std::string>(match.begin(), match.end());
  }
  return {};
}

std::unique_ptr<IRegexSearch> CreateRegexSearch(const std::string& pattern) {
  return std::make_unique<RegexSearch>(pattern);
}
}  // namespace Core