#pragma once

#include "core/interface/json_interface.h"

#include <memory>
#include <string>
#include <vector>

namespace Core {
class SimpleJsonParser final : public IJsonParser {
 public:
  SimpleJsonParser();
  virtual ~SimpleJsonParser();
  virtual void Parse(const std::string& content) override;
  virtual std::string GetString(
      const std::string& key) override;
  virtual int64_t GetInt64(const std::string& key) override;
  virtual std::vector<std::string> GetArrayString(
      const std::string& key) override;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
}  // namespace Core