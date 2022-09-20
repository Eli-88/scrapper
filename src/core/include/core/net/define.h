#pragma once

#include <boost/beast/http.hpp>

namespace Core {
namespace Net {

using HttpVerb = boost::beast::http::verb;
const auto HttpGet = boost::beast::http::verb::get;
const auto HttpPost = boost::beast::http::verb::post;

using HttpRequest = boost::beast::http::request<boost::beast::http::string_body>;
using HttpResponse = boost::beast::http::response<boost::beast::http::string_body>;

}  // namespace Net
}  // namespace Core
