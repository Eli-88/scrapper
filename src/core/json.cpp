#include "core/json.h"

#include <algorithm>

#define SIMPLE_JSON_IMPL(context) impl_->context

namespace Core {
JsonValue ParseJson(const std::string& content) {
  Core::Json::parser parser;
  Core::Json::error_code ec;
  parser.write(content, ec);

  if (ec) {
    throw std::runtime_error("parse json fails");
  }

  return parser.release();
}

struct SimpleJsonParser::Impl {
  Json::object obj;
};

SimpleJsonParser::SimpleJsonParser() : impl_(std::make_unique<Impl>()) {}

SimpleJsonParser::~SimpleJsonParser() {}

void SimpleJsonParser::Parse(const std::string& content) {
  SIMPLE_JSON_IMPL(obj) = ParseJson(content).as_object();
}

std::string SimpleJsonParser::GetString(const std::string& key) {
  auto iter = SIMPLE_JSON_IMPL(obj).find(key);
  if (iter == SIMPLE_JSON_IMPL(obj).end()) {
    return {};
  }
  const auto& value = iter->value();
  if (!value.is_string()) {
    throw std::runtime_error("string parameter is required for this json key");
  }
  const auto& result = value.get_string();
  return std::string(result.begin(), result.end());
}

int64_t SimpleJsonParser::GetInt64(const std::string& key) {
  auto iter = SIMPLE_JSON_IMPL(obj).find(key);
  if (iter == SIMPLE_JSON_IMPL(obj).end()) {
    return {};
  }
  const auto& value = iter->value();
  return value.as_int64();
}

std::vector<std::string> SimpleJsonParser::GetArrayString(
    const std::string& key) {
  auto iter = SIMPLE_JSON_IMPL(obj).find(key);
  if (iter == SIMPLE_JSON_IMPL(obj).end()) {
    return {};
  }
  const auto& value = iter->value();
  const auto& arr = value.as_array();
  std::vector<std::string> retval{};

  for (auto i = arr.begin(); i != arr.end(); i++) {
    if (!i->is_string()) {
      throw std::runtime_error(
          "element in this json array is not found to be string");
    }
    const auto& str = i->get_string();
    retval.emplace_back(std::string(str.begin(), str.end()));
  }
  return retval;
}

}  // namespace Core