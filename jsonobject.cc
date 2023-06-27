#include "jsonobject.hh"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <sstream>

namespace json {
JsonObject::JsonObject() : m_obj(nullptr), m_type(Jtype::T_NULL) {}

JsonObject::JsonObject(str_t str) : m_obj(str), m_type(Jtype::T_STR) {}

JsonObject::JsonObject(double_t d) : m_obj(d), m_type(Jtype::T_DOUBLE) {}

JsonObject::JsonObject(int_t i) : m_obj(i), m_type(Jtype::T_INT) {}

JsonObject::JsonObject(list_t list) : m_obj(list), m_type(Jtype::T_LIST) {}

JsonObject::JsonObject(dict_t dict) : m_obj(dict), m_type(Jtype::T_DICT) {}

template <typename T> auto JsonObject::getValue() -> std::optional<T> {
  if constexpr (std::is_same_v<T, null_t>) {
    return m_type == Jtype::T_NULL ? std::optional<T>{null_t{}} : std::nullopt;
  } else if constexpr (std::is_same_v<T, str_t>) {
    return m_type == Jtype::T_STR ? std::optional<T>{std::get<str_t>(m_obj)}
                                  : std::nullopt;
  } else if constexpr (std::is_same_v<T, int_t>) {
    return m_type == Jtype::T_INT ? std::optional<T>{std::get<int_t>(m_obj)}
                                  : std::nullopt;
  } else if constexpr (std::is_same_v<T, double_t>) {
    return m_type == Jtype::T_DOUBLE
               ? std::optional<T>{std::get<double_t>(m_obj)}
               : std::nullopt;
  } else if constexpr (std::is_same_v<T, list_t>) {
    return m_type == Jtype::T_LIST ? std::optional<T>{std::get<list_t>(m_obj)}
                                   : std::nullopt;
  } else if constexpr (std::is_same_v<T, dict_t>) {
    return m_type == Jtype::T_DICT ? std::optional<T>{std::get<dict_t>(m_obj)}
                                   : std::nullopt;
  }
  // cannot reach
  return std::nullopt;
}

template <typename T> auto JsonObject::getValuePtr() -> T * {
  if constexpr (std::is_same_v<T, null_t>) {
    return nullptr;
  } else if constexpr (std::is_same_v<T, str_t>) {
    return m_type == Jtype::T_STR ? std::get_if<str_t>(&m_obj) : nullptr;
  } else if constexpr (std::is_same_v<T, int_t>) {
    return m_type == Jtype::T_INT ? std::get_if<int_t>(&m_obj) : nullptr;
  } else if constexpr (std::is_same_v<T, double_t>) {
    return m_type == Jtype::T_DOUBLE ? std::get_if<double_t>(&m_obj) : nullptr;
  } else if constexpr (std::is_same_v<T, list_t>) {
    return m_type == Jtype::T_LIST ? std::get_if<list_t>(&m_obj) : nullptr;
  } else if constexpr (std::is_same_v<T, dict_t>) {
    return m_type == Jtype::T_DICT ? std::get_if<dict_t>(&m_obj) : nullptr;
  }
  return nullptr;
}

auto JsonObject::operator[](const std::string &key) -> JsonObject & {
  if (m_type != Jtype::T_DICT)
    throw std::invalid_argument(
        "JsonObject::operator should be either 'dict_t'");

  return std::get<dict_t>(m_obj)[key];
}

template <> auto JsonObject::toString<null_t>() -> std::string const {
  return "null";
}

template <> auto JsonObject::toString<str_t>() -> std::string const {
  return "\"" + *getValuePtr<str_t>() + "\"";
}

template <> auto JsonObject::toString<int_t>() -> std::string const {
  int i = *getValuePtr<int_t>();
  return std::to_string(i);
}

template <> auto JsonObject::toString<double_t>() -> std::string const {
  double d = *getValuePtr<double_t>();
  return std::to_string(d);
}

template <> auto JsonObject::toString<list_t>() -> std::string const {
  std::stringstream ss;
  const auto p = getValuePtr<list_t>();

  ss << "[";
  for (auto it = p->begin(); it != p->end(); ++it) {
    auto tmp = *it;
    ss << tmp.toString();
    // the last one
    ++it;
    if (it != p->end())
      ss << ",";
    --it;
  }
  ss << "]";

  return ss.str();
}

template <> auto JsonObject::toString<dict_t>() -> std::string const {
  std::stringstream ss;
  const auto p = getValuePtr<dict_t>();

  ss << "{";
  for (auto it = p->begin(); it != p->end(); ++it) {
    ss << it->first;
    ss << ":";
    ss << it->second.toString();
    // the last one
    ++it;
    if (it != p->end())
      ss << ",";
    --it;
  }
  ss << "}";

  return ss.str();
}

template <typename T> auto JsonObject::toString() -> std::string const {
  throw std::runtime_error("Unknown json type");
  // unreachable
  return "";
}

auto JsonObject::toString() -> std::string const {
  switch (m_type) {
  case T_NULL:
    assert(std::get_if<null_t>(&m_obj) != nullptr);
    return toString<null_t>();
  case T_STR:
    assert(std::get_if<str_t>(&m_obj) != nullptr);
    return toString<str_t>();
  case T_INT:
    assert(std::get_if<int_t>(&m_obj) != nullptr);
    return toString<int_t>();
  case T_DOUBLE:
    assert(std::get_if<double_t>(&m_obj) != nullptr);
    return toString<double_t>();
  case T_LIST:
    assert(std::get_if<list_t>(&m_obj) != nullptr);
    return toString<list_t>();
  case T_DICT:
    assert(std::get_if<dict_t>(&m_obj) != nullptr);
    return toString<dict_t>();
  default:
    return toString<null_t>();
  }
  // cannot reach
  return "";
}

} // namespace json