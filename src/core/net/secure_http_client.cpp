#include "secure_http_client.h"

#include "core/logger.h"

namespace Core {
namespace Net {

namespace beast = boost::beast;    // from <boost/beast.hpp>
namespace http = beast::http;      // from <boost/beast/http.hpp>
namespace net = boost::asio;       // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;  // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>
using io_context = boost::asio::io_context;

SecureHttpClientSession::SecureHttpClientSession(
    net::any_io_executor ex,
    ssl::context& ssl_context,
    IHttpResponseHandler& responseHandler)
    : resolver_(ex),
      stream_(ex, ssl_context),
      responseHandler_(responseHandler) {}

void SecureHttpClientSession::Send(http::verb httpVerb,
                                   const std::string& body,
                                   char const* host,
                                   char const* port,
                                   char const* target,
                                   int version) {
  // Set SNI Hostname (many hosts need this to handshake successfully)
  if (!SSL_set_tlsext_host_name(stream_.native_handle(), host)) {
    beast::error_code ec{static_cast<int>(::ERR_get_error()),
                         net::error::get_ssl_category()};
    return;
  }

  // Set up an HTTP request message
  req_.version(version);
  req_.method(httpVerb);
  req_.target(target);
  req_.body() = body;
  req_.set(http::field::connection, "close");
  req_.set(http::field::host, host);
  req_.set(http::field::user_agent, "Mozilla/5.0 (X11; Linux x86_64; rv:12.0)");
  req_.set(http::field::accept, "*/*");
  req_.set(http::field::cookie,
           "CTK=1g9p1ta53k44k800; "
           "JSESSIONID=4CC5A279052E9BF4522D910180BA5947; ctkgen=1; mdserp=1");

  req_.prepare_payload();

  LOG_DEBUG("sending requests to %s:%s/%s", host, port, target);

  // Look up the domain name
  resolver_.async_resolve(
      host, port,
      beast::bind_front_handler(&SecureHttpClientSession::OnResolve,
                                shared_from_this()));
}

void SecureHttpClientSession::OnResolve(
    boost::beast::error_code ec,
    boost::asio::ip::tcp::resolver::results_type results) {
  if (ec) {
    LOG(ERROR, "client resolve error: %s", ec.message().c_str());
    return;
  }

  // Set a timeout on the operation
  beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

  // Make the connection on the IP address we get from a lookup
  beast::get_lowest_layer(stream_).async_connect(
      results, beast::bind_front_handler(&SecureHttpClientSession::OnConnect,
                                         shared_from_this()));
}

void SecureHttpClientSession::OnConnect(
    beast::error_code ec,
    tcp::resolver::results_type::endpoint_type) {
  if (ec) {
    LOG(ERROR, "client connection error: %s", ec.message().c_str());
    return;
  }

  // Perform the SSL handshake
  stream_.async_handshake(
      ssl::stream_base::client,
      beast::bind_front_handler(&SecureHttpClientSession::OnHandshake,
                                shared_from_this()));
}

void SecureHttpClientSession::OnHandshake(beast::error_code ec) {
  if (ec) {
    LOG(ERROR, "ssl handshake error: %s", ec.message().c_str());
    return;
  }

  // Set a timeout on the operation
  beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

  // Send the HTTP request to the remote host
  http::async_write(stream_, req_,
                    beast::bind_front_handler(&SecureHttpClientSession::OnWrite,
                                              shared_from_this()));
}

void SecureHttpClientSession::OnWrite(beast::error_code ec,
                                      std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  if (ec) {
    LOG(ERROR, "client send error: %s", ec.message().c_str());
    return;
  }

  DoRead();
}

void SecureHttpClientSession::DoRead() {
  // Receive the HTTP response
  http::async_read(stream_, buffer_, res_,
                   beast::bind_front_handler(&SecureHttpClientSession::OnRead,
                                             shared_from_this()));
}

void SecureHttpClientSession::OnRead(beast::error_code ec,
                                     std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);
  LOG(TRACE, "client on read");
  if (ec) {
    LOG(TRACE, "client read ends: %s", ec.message().c_str());
    return;
  }

  responseHandler_.OnHttpResponse(std::move(res_.body()));
  // Set a timeout on the operation
  beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(5));

  DoRead();

  // Gracefully close the stream
  return stream_.async_shutdown(beast::bind_front_handler(
      &SecureHttpClientSession::OnShutDown, shared_from_this()));
}

void SecureHttpClientSession::OnShutDown(beast::error_code ec) {
  if (ec == net::error::eof) {
    LOG(TRACE, "client session end of file: %s", ec.message().c_str());
    // Rationale:
    // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
    ec = {};
  }
  if (ec) {
    return;
  }

  // If we get here then the connection is closed gracefully
}

std::shared_ptr<IClient> CreateSecureHttpClient(
    IContextManager& contextManager,
    IHttpResponseHandler& responseHandler) {
  auto& ioCtx = contextManager.IoContext();
  auto& sslContext = contextManager.SslContext();
  return std::make_shared<SecureHttpClientSession>(net::make_strand(ioCtx),
                                                   sslContext, responseHandler);
}

}  // namespace Net
}  // namespace Core