#pragma once

#include <memory>
#include "core/interface/net/request_handler_interface.h"

namespace Core {
namespace Net {
class IServer {
 public:
  virtual ~IServer() = default;
  virtual void Run() = 0;
};

std::unique_ptr<IServer> CreateHttpServer(
    Core::Net::IHttpRequestHandlerFactory& requestHandlerFactory,
    const char* host,
    unsigned short port);

}  // namespace Net
}  // namespace Core