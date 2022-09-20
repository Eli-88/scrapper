#pragma once

#include "core/net/define.h"

#include <string>

namespace Core {
namespace Net {
class IHttpResponseHandler {
 public:
  virtual ~IHttpResponseHandler() = default;
  virtual void OnHttpResponse(std::string&& body) = 0;
};
}  // namespace Net
}  // namespace Core