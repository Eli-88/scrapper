#include "http_server.h"
#include "core/interface/net/server_interface.h"
#include "core/logger.h"

#include <exception>

namespace Core {
namespace Net {

namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

HttpSession::HttpSession(boost::asio::ip::tcp::socket&& socket,
                         IHttpRequestHandlerFactory& requestHandlerFactory)
    : stream_(std::move(socket)),
      requestHandlerFactory_(requestHandlerFactory) {}

void HttpSession::Run() {
  boost::asio::dispatch(stream_.get_executor(),
                        beast::bind_front_handler(&HttpSession::DoRead,
                                                  this->shared_from_this()));
}

void HttpSession::DoRead() {
  // Make the request empty before reading,
  // otherwise the operation behavior is undefined.
  req_ = {};

  // Set the timeout.
  stream_.expires_after(std::chrono::seconds(10));

  // Read a request
  http::async_read(stream_, buffer_, req_,
                   beast::bind_front_handler(&HttpSession::OnRead,
                                             this->shared_from_this()));
}

void HttpSession::OnRead(beast::error_code ec, std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  // This means they closed the connection
  if (ec == http::error::end_of_stream) {
    LOG(TRACE, "read session end of stream: %s", ec.message().c_str());
    return DoClose();
  }

  if (ec) {
    LOG(TRACE, "session fail to read: %s", ec.message().c_str());
    return;
  }

  const auto body = req_.body();

  auto response = requestHandlerFactory_.Create(req_).OnHttpRequest(body);

  DoWrite(response);
}

void HttpSession::DoWrite(const http::response<http::string_body>& res) {
  LOG(TRACE, "replying request: %s", res.body().c_str());
  // Write the response
  http::async_write(stream_, res,
                    beast::bind_front_handler(&HttpSession::OnWrite,
                                              this->shared_from_this()));
}

void HttpSession::OnWrite(beast::error_code ec, std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  if (ec) {
    LOG(TRACE, "session write fail: %s", ec.message().c_str());
    return;
  }

  // TODO: wrap this in a strategy class to check for close for http 1.0, 1.1
  auto& header = req_.base();
  auto connection_status = header.find(http::field::connection);
  if (connection_status != header.end() &&
      connection_status->value() == "close") {
    return;
  }
  DoRead();
}

void HttpSession::DoClose() {
  LOG(TRACE, "session closing");
  // Send a TCP shutdown
  beast::error_code ec;
  stream_.socket().shutdown(tcp::socket::shutdown_send, ec);

  // At this point the connection is closed gracefully
}

HttpServer::HttpServer(boost::asio::ip::tcp::endpoint endpoint,
                       IHttpRequestHandlerFactory& requestHandlerFactory)
    : acceptor_(ioContext_), requestHandlerFactory_(requestHandlerFactory) {
  // Open the acceptor
  beast::error_code ec;
  acceptor_.open(endpoint.protocol(), ec);
  if (ec) {
    LOG(ERROR, "acceptor fail to open: %s", ec.message().c_str());
    throw std::runtime_error(ec.what());
  }

  acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);

  // Bind to the server address
  acceptor_.bind(endpoint, ec);
  if (ec) {
    LOG(ERROR, "acceptor fail to bind: %s", ec.message().c_str());
    throw std::runtime_error(ec.what());
  }

  // Start listening for connections
  acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
  if (ec) {
    LOG(ERROR, "acceptor fail to listen: %s", ec.message().c_str());
    throw std::runtime_error(ec.what());
  }
}

// Start accepting incoming connections
void HttpServer::Run() {
  LOG(TRACE, "server starting...");
  DoAccept();
  ioContext_.run();
}

void HttpServer::DoAccept() {
  // The new connection gets its own strand
  acceptor_.async_accept(
      ioContext_, beast::bind_front_handler(&HttpServer::OnAccept, this));
}

void HttpServer::OnAccept(beast::error_code ec, tcp::socket socket) {
  if (ec) {
    LOG(ERROR, "fail to accept connection: %s", ec.message().c_str());
    return;  // To avoid infinite loop
  } else {
    LOG(TRACE, "create a new session");
    // Create the session and run it
    std::make_shared<HttpSession>(std::move(socket), requestHandlerFactory_)
        ->Run();
  }

  // Accept another connection
  DoAccept();
}

std::unique_ptr<IServer> CreateHttpServer(
    IHttpRequestHandlerFactory& requestHandlerFactory,
    const char* host,
    unsigned short port) {
  return std::make_unique<HttpServer>(
      tcp::endpoint(boost::asio::ip::make_address_v4(host), port),
      requestHandlerFactory);
}
}  // namespace Net
}  // namespace Core