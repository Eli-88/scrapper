#include "core/algorithm.h"
#include <boost/algorithm/string.hpp>

namespace Core {
std::vector<std::string> SplitString(const std::string& target,
                                     const std::string& delimiter) {
  std::vector<std::string> result;
  boost::split(result, target, boost::is_any_of(delimiter));
  return result;
}
}  // namespace Core