#pragma once

#include <string>
#include "core/net/define.h"

namespace Core {
namespace Net {
class IHttpRequestHandler {
 public:
  virtual ~IHttpRequestHandler() = default;
  virtual Core::Net::HttpResponse OnHttpRequest(const std::string& body) = 0;
};

class IHttpRequestHandlerFactory {
 public:
  virtual ~IHttpRequestHandlerFactory() = default;

  virtual Core::Net::IHttpRequestHandler& Create(
      const Core::Net::HttpRequest& httpRequest) = 0;
      
  virtual void RegisterHandler(Core::Net::HttpVerb httpVerb,
                               const std::string& target,
                               Core::Net::IHttpRequestHandler& handler) = 0;
};
}  // namespace Net
}  // namespace Core