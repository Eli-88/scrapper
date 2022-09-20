#include "core/net/http_request_handler.h"
#include "core/net/define.h"

#include <array>
#include <iostream>
#include <unordered_map>

namespace Core {
namespace Net {

struct HandleBadRequest : public IHttpRequestHandler {
  virtual HttpResponse OnHttpRequest(const std::string& /*body*/) override {
    // TODO: need to handle probably
    HttpResponse response{};
    response.body() = "bad request";
    return response;
  }
};

using HandlerFactoryMap =
    std::array<std::unordered_map<std::string,
                                  std::reference_wrapper<IHttpRequestHandler>>,
               100>;  // TODO: replace HttpVerb with own enum class to wrap the
                      // beast library and provide a max enum here to prevent
                      // overflow

struct HttpRequestHandlerFactory::Impl {
  HandlerFactoryMap abstractFactory{};
  HandleBadRequest exceptionHandler{};
};

HttpRequestHandlerFactory::HttpRequestHandlerFactory()
    : impl_(std::make_unique<HttpRequestHandlerFactory::Impl>()) {}

HttpRequestHandlerFactory::~HttpRequestHandlerFactory() {}

void HttpRequestHandlerFactory::RegisterHandler(HttpVerb httpVerb,
                                                const std::string& target,
                                                IHttpRequestHandler& handler) {
  auto& factory = impl_->abstractFactory.at(std::size_t(httpVerb));
  factory.insert({target, std::ref(handler)});
}

IHttpRequestHandler& HttpRequestHandlerFactory::Create(
    const HttpRequest& httpRequest) {
  const HttpVerb httpVerb = httpRequest.method();
  auto& factory = impl_->abstractFactory.at(std::size_t(httpVerb));
  auto result = factory.find(httpRequest.target().to_string());

  // no match
  if (result == factory.end()) {
    return impl_->exceptionHandler;
  }

  return result->second.get();
}

}  // namespace Net
}  // namespace Core