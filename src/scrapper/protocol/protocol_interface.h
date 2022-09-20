#pragma once

#include <memory>

namespace Scrapper {
namespace Protocol {

class Registration;
class HealthCheck;

class IProtocolVisitor {
 public:
  virtual ~IProtocolVisitor() = default;
  virtual bool OnRegistration(Protocol::Registration& /*msg*/) { return false; }
  virtual bool OnHealthCheck(Protocol::HealthCheck&) { return false; }
};

class IProtocol {
 public:
  virtual ~IProtocol() = default;
  virtual bool Accept(IProtocolVisitor& /*visitor*/) { return false; }
};

class IDecoder {
 public:
  virtual ~IDecoder() = default;
  virtual std::unique_ptr<IProtocol> Decode(const std::string& rawMessage) = 0;
};

}  // namespace Protocol
}  // namespace Scrapper