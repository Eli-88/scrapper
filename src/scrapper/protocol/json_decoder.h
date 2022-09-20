#pragma once

#include <core/interface/json_interface.h>
#include "protocol_interface.h"

#include "healthcheck.h"
#include "registration.h"

#include <unordered_map>

namespace Scrapper {
namespace Protocol {

class JsonRegistrationFactory : public IRegistrationFactory {
 public:
  JsonRegistrationFactory();
  virtual Registration Create(const std::string& rawMessage);

 private:
  std::unique_ptr<Core::IJsonParser> parser_;
};

class JsonHealthCheckFactory : public IHealthCheckFactory {
 public:
  virtual ~JsonHealthCheckFactory() = default;
  virtual HealthCheck Create(const std::string& rawMessage);
};

}  // namespace Protocol
}  // namespace Scrapper