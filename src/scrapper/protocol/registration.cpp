#include "registration.h"

namespace Scrapper {
namespace Protocol {
Registration::Registration(const std::string& host,
                           const std::string& port,
                           const std::string& target,
                           const std::string& version,
                           const RegexCollection& regexPatterns)
    : host_(host),
      port_(port),
      target_(target),
      version_(version == "1.0" ? 10 : 11),
      regexPatterns_(regexPatterns) {}

const std::string Registration::Host() const {
  return host_;
}

const std::string Registration::Port() const {
  return port_;
}

const std::string Registration::Target() const {
  return target_;
}

int Registration::Version() const {
  return version_;
}

const Registration::RegexCollection& Registration::RegexPatterns() const {
  return regexPatterns_;
}

}  // namespace Protocol
}  // namespace Scrapper