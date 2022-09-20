#include "core/net/context_manager.h"
#include "core/logger.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/context.hpp>

namespace Core {
namespace Net {

struct ContextManager::Impl {
  boost::asio::io_context ioCtx;
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
      ioGuard;
  boost::asio::ssl::context sslCtx{
      boost::asio::ssl::context::tlsv12_client};  // TODO: extract this so it
                                                  // can be reuse for server
  Impl() : ioGuard(ioCtx.get_executor()) {}
};

ContextManager::ContextManager() : impl_(std::make_unique<Impl>()) {}
ContextManager::~ContextManager() {}
boost::asio::io_context& ContextManager::IoContext() {
  return impl_->ioCtx;
}

boost::asio::ssl::context& ContextManager::SslContext() {
  return impl_->sslCtx;
}

void ContextManager::Run() {
  LOG_TRACE();
  impl_->ioCtx.run();
}
}  // namespace Net
}  // namespace Core