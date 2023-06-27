#pragma once

#include <list>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

namespace json {

class JsonObject;
using null_t = std::nullptr_t;
using int_t = int32_t;
using double_t = double;
using str_t = std::string;
using list_t = std::list<JsonObject>;
using dict_t = std::map<std::string, JsonObject>;
// using dict_t = std::unordered_map<std::string, JsonObject>;

enum Jtype {
  T_NULL,
  T_INT,
  T_DOUBLE,
  T_STR,
  T_LIST,
  T_DICT,
};

class JsonObject {
public:
  using jobj_t = std::variant<null_t, str_t, int_t, double_t, list_t, dict_t>;
  using type_t = Jtype;

  JsonObject();
  JsonObject(str_t str);
  JsonObject(int_t i);
  JsonObject(double_t d);
  JsonObject(list_t list);
  JsonObject(dict_t dict);

  auto toString() -> std::string const;

  template <typename T>
  [[deprecated("This function is deprecated. Please use the getValuePtr() "
               "instead.")]] auto
  getValue() -> std::optional<T>;

  template <typename T> auto getValuePtr() -> T *;

  auto operator[](const std::string &key) -> JsonObject &;

private:
  template <typename T> auto toString() -> std::string const;

private:
  jobj_t m_obj;
  type_t m_type;
};

} // namespace json
