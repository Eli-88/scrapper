#pragma once

#include "core/interface/net/context_manager_interface.h"

#include <memory>

namespace Core {
namespace Net {
class ContextManager : public IContextManager {
 public:
  ContextManager();
  virtual ~ContextManager();
  virtual boost::asio::io_context& IoContext() override;
  virtual boost::asio::ssl::context& SslContext() override;
  virtual void Run() override;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
}  // namespace Net
}  // namespace Core