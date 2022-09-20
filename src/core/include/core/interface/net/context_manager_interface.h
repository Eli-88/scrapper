#pragma once

// forward declaration
namespace boost {
namespace asio {
class io_context;
namespace ssl {
class context;
}
}  // namespace asio
}  // namespace boost

namespace Core {
namespace Net {
class IContextManager {
 public:
  virtual ~IContextManager() = default;
  virtual boost::asio::io_context& IoContext() = 0;
  virtual boost::asio::ssl::context& SslContext() = 0;
  virtual void Run() = 0;
};
}  // namespace Net
}  // namespace Core