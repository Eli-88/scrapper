#pragma once

#include "core/net/define.h"
#include "core/interface/net/response_handler_interface.h"
#include "core/interface/net/context_manager_interface.h"

#include <memory>

namespace Core {
namespace Net {

class IClient {
 public:
  virtual ~IClient() = default;

  virtual void Send(HttpVerb httpVerb,
                    const std::string& body,
                    char const* host,
                    char const* port,
                    char const* target,
                    int version) = 0;
};

std::shared_ptr<IClient> CreateSecureHttpClient(
    IContextManager& contextManager,
    IHttpResponseHandler& responseHandler);

}  // namespace Net
}  // namespace Core