#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "jsonobject.hh"

#define START_STRUCT(name) struct name {
#define END_STRUCT(name)                                                       \
  }                                                                            \
  ;

namespace json {
class JsonParser {
public:
  friend class JsonObject;
  // using jobj_t = json::JsonObject::jobj_t;

  JsonParser() = default;
  ~JsonParser() = default;

  auto fromJson(std::string &&json_data) -> JsonObject;
  auto toJson(JsonObject &&jsonobject) -> std::string;
  virtual auto serialize(JsonObject &&jsonobject) -> std::string;

  // auto operator[](const std::string_view& name) -> JsonObject;
  // auto operator[](const std::string& name) -> JsonObject;
private:
  auto get_next_token() -> char;
  void skip_comment();
  auto parse() -> JsonObject;

  auto parse_null() -> JsonObject;
  auto parse_string() -> JsonObject;
  auto parse_number() -> JsonObject;
  auto parse_list() -> JsonObject;
  auto parse_dict() -> JsonObject;

private:
  std::string m_data;
  size_t m_pos{0};
};

} // namespace json
