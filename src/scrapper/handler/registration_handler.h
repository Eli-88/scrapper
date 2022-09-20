#pragma once

#include "engine/scrape_manager_interface.h"
#include "protocol/protocol_interface.h"
#include "protocol/registration.h"

#include <core/interface/json_interface.h>
#include <core/interface/net/request_handler_interface.h>

#include <memory>

namespace Scrapper {
namespace Handler {
class RegistrationHandler final : public Core::Net::IHttpRequestHandler {
 public:
  explicit RegistrationHandler(IScrapeManager& scrapeManager);

  virtual Core::Net::HttpResponse OnHttpRequest(
      const std::string& body) override;

 private:
  std::unique_ptr<Protocol::IRegistrationFactory> registrationFactory_;
  IScrapeManager& scrapeManager_;
};
}  // namespace Handler
}  // namespace Scrapper