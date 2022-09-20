#pragma once

#include <boost/json.hpp>

namespace Core {

namespace Json = boost::json;
using JsonValue = boost::json::value;
const auto JsonObjKind = boost::json::kind::object;
const auto JsonArrKind = boost::json::kind::array;

JsonValue ParseJson(const std::string& content);

class IJsonParser {
 public:
  virtual ~IJsonParser() = default;
  virtual void Parse(const std::string& content) = 0;
  virtual std::string GetString(const std::string& key) = 0;
  virtual int64_t GetInt64(const std::string& key) = 0;
  virtual std::vector<std::string> GetArrayString(const std::string& key) = 0;
};
}  // namespace Core