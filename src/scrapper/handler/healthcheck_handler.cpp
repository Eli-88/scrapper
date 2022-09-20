#include "healthcheck_handler.h"
#include "protocol/json_decoder.h"

namespace Net = Core::Net;

namespace Scrapper {
namespace Handler {

HealthCheckHandler::HealthCheckHandler()
    : healthcheckFactory_(std::make_unique<Protocol::JsonHealthCheckFactory>()) {}

Net::HttpResponse HealthCheckHandler::OnHttpRequest(
    const std::string& /*body*/) {
  Net::HttpResponse response;
  response.body() = "Health Check Ok";
  return response;
}
}  // namespace Handler
}  // namespace Scrapper