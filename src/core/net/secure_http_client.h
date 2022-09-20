#pragma once

#include "core/interface/net/client_interface.h"
#include "core/interface/net/response_handler_interface.h"

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>

#include <memory>
#include <string>

namespace Core {
namespace Net {
class SecureHttpClientSession
    : public std::enable_shared_from_this<SecureHttpClientSession>,
      public IClient {
 public:
  SecureHttpClientSession(boost::asio::any_io_executor ex,
                      boost::asio::ssl::context& ssl_context,
                      IHttpResponseHandler& responseHandler);

  virtual void Send(boost::beast::http::verb httpVerb,
            const std::string& body,
            char const* host,
            char const* port,
            char const* target,
            int version) override;

 private:
  boost::asio::ip::tcp::resolver resolver_;
  boost::beast::ssl_stream<boost::beast::tcp_stream> stream_;
  boost::beast::flat_buffer buffer_;
  boost::beast::http::request<boost::beast::http::string_body> req_;
  boost::beast::http::response<boost::beast::http::string_body> res_;
  IHttpResponseHandler& responseHandler_;

  void OnResolve(boost::beast::error_code ec,
                 boost::asio::ip::tcp::resolver::results_type results);

  void OnConnect(boost::beast::error_code ec,
                 boost::asio::ip::tcp::resolver::results_type::endpoint_type);

  void OnHandshake(boost::beast::error_code ec);
  void OnWrite(boost::beast::error_code ec, std::size_t bytes_transferred);
  void DoRead();

  void OnRead(boost::beast::error_code ec, std::size_t bytes_transferred);

  void OnShutDown(boost::beast::error_code ec);
};
}  // namespace Net
}  // namespace Core