#pragma once

namespace Scrapper {
class IConfig {
 public:
  virtual ~IConfig() = default;
  virtual const char* Host() const = 0;
  virtual unsigned short Port() const = 0;
};
}  // namespace Scrapper