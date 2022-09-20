#pragma once

#include "core/interface/net/request_handler_interface.h"
#include "core/interface/net/server_interface.h"

#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
#include <memory>

namespace Core {
namespace Net {

class HttpSession : public std::enable_shared_from_this<HttpSession> {
 public:
  HttpSession(boost::asio::ip::tcp::socket&& socket,
              IHttpRequestHandlerFactory& requestHandler);

  void Run();

 private:
  boost::beast::tcp_stream stream_;
  IHttpRequestHandlerFactory& requestHandlerFactory_;
  boost::beast::flat_buffer buffer_;
  boost::beast::http::request<boost::beast::http::string_body> req_{};

  void DoRead();
  void OnRead(boost::beast::error_code ec, std::size_t bytes_transferred);
  void DoWrite(
      const boost::beast::http::response<boost::beast::http::string_body>& res);
  void DoClose();
  void OnWrite(boost::beast::error_code ec, std::size_t bytes_transferred);
};

class HttpServer final : public IServer {
 public:
  HttpServer(boost::asio::ip::tcp::endpoint endpoint,
             IHttpRequestHandlerFactory& requestHandler);

  // Start accepting incoming connections
  virtual void Run() override;

 private:
  void DoAccept();

  void OnAccept(boost::beast::error_code ec,
                boost::asio::ip::tcp::socket socket);

 private:
  boost::asio::io_context ioContext_;
  boost::asio::ip::tcp::acceptor acceptor_;
  IHttpRequestHandlerFactory& requestHandlerFactory_;
};
}  // namespace Net
}  // namespace Core