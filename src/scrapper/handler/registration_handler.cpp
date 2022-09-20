#include "registration_handler.h"
#include "protocol/json_decoder.h"

#include <core/json.h>
#include <core/logger.h>

#include <iostream>

namespace Scrapper {
namespace Handler {

RegistrationHandler::RegistrationHandler(IScrapeManager& scrapeManager)
    : registrationFactory_(
          std::make_unique<Protocol::JsonRegistrationFactory>()),
      scrapeManager_(scrapeManager) {}

Core::Net::HttpResponse RegistrationHandler::OnHttpRequest(
    const std::string& body) {
  LOG_TRACE();
  Core::Net::HttpResponse response;
  try {
    auto msg = registrationFactory_->Create(body);

    scrapeManager_.Dispatch(msg.Host(), msg.Port(), msg.Target(), msg.Version(),
                            msg.RegexPatterns());
    response.body() = "Ok";

  } catch (const std::exception& ec) {
    LOG_ERROR("%s", ec.what());
    response.body() = "invalid request";
  }
  return response;
}

}  // namespace Handler
}  // namespace Scrapper