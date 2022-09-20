#pragma once

#include <string>
#include <vector>

namespace Scrapper {
namespace Protocol {

class Registration {
 public:
  using RegexCollection = std::vector<std::string>;
  Registration(const std::string& host,
               const std::string& port,
               const std::string& target,
               const std::string& version,
               const RegexCollection& regexPatterns);
  ~Registration() = default;

  const std::string Host() const;
  const std::string Port() const;
  const std::string Target() const;
  int Version() const;
  const RegexCollection& RegexPatterns() const;

 private:
  const std::string host_;
  const std::string port_;
  const std::string target_;
  const int version_;
  RegexCollection regexPatterns_{};
};

class IRegistrationFactory {
 public:
  virtual ~IRegistrationFactory() = default;
  virtual Registration Create(
      const std::string& rawMessage) = 0;
};

}  // namespace Protocol
}  // namespace Scrapper