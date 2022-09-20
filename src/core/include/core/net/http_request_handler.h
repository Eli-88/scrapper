#pragma once

#include <memory>
#include "core/interface/net/request_handler_interface.h"

namespace Core {
namespace Net {
class HttpRequestHandlerFactory final : public IHttpRequestHandlerFactory {
 public:
  HttpRequestHandlerFactory();
  virtual ~HttpRequestHandlerFactory();
  virtual void RegisterHandler(HttpVerb httpVerb,
                               const std::string& target,
                               IHttpRequestHandler& handler) override;
  virtual IHttpRequestHandler& Create(const HttpRequest& httpRequest) override;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
}  // namespace Net
}  // namespace Core
