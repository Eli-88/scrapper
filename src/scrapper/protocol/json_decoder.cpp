#include "json_decoder.h"

#include <core/json.h>
#include <core/logger.h>

#include <iostream>

namespace Scrapper {
namespace Protocol {

JsonRegistrationFactory::JsonRegistrationFactory()
    : parser_(std::make_unique<Core::SimpleJsonParser>()) {}

Registration JsonRegistrationFactory::Create(const std::string& rawMessage) {
  parser_->Parse(rawMessage);
  const auto host = parser_->GetString("host");
  const auto port = parser_->GetString("port");
  const auto target = parser_->GetString("target");
  const auto version = parser_->GetString("version");
  const auto regexPatterns = parser_->GetArrayString("patterns");

  LOG_DEBUG("host: %s, port: %s, target: %s, version: %s", host.c_str(),
            port.c_str(), target.c_str(), version.c_str());
  return Registration(host, port, target, version, regexPatterns);
}

HealthCheck JsonHealthCheckFactory::Create(const std::string& /*rawMessage*/) {
  return HealthCheck{};
}

}  // namespace Protocol
}  // namespace Scrapper