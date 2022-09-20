#pragma once

#include "protocol_interface.h"

#include <string>
#include <vector>

namespace Scrapper {
namespace Protocol {

class HealthCheck {
 public:
  ~HealthCheck() = default;
  HealthCheck() = default;
};

class IHealthCheckFactory {
 public:
  virtual ~IHealthCheckFactory() = default;
  virtual HealthCheck Create(const std::string&) = 0;
};

}  // namespace Protocol
}  // namespace Scrapper