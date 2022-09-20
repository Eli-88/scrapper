#pragma once

#include <core/interface/net/request_handler_interface.h>
#include "protocol/healthcheck.h"

#include <memory>

namespace Scrapper {
namespace Handler {
class HealthCheckHandler : public Core::Net::IHttpRequestHandler {
 public:
  HealthCheckHandler();
  virtual Core::Net::HttpResponse OnHttpRequest(
      const std::string& body) override;

 private:
  std::unique_ptr<Protocol::IHealthCheckFactory> healthcheckFactory_;
};
}  // namespace Handler
}  // namespace Scrapper