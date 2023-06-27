// #include "jsonobject.hh"
#include <cassert>
#include <iostream>
#include <type_traits>

#include "jsonparser.hh"

using namespace std;
using namespace json;

template <typename T> void print(T *const value) {
  if constexpr (std::is_same_v<T, null_t>) {
    cout << (value ? "?" : "nullptr") << endl;
  } else if constexpr (std::is_same_v<T, str_t>) {
    cout << *value << endl;
  } else if constexpr (std::is_same_v<T, int_t>) {
    cout << *value << endl;
  } else if constexpr (std::is_same_v<T, double_t>) {
    cout << *value << endl;
  } else if constexpr (std::is_same_v<T, list_t>) {
    for (auto it = value->begin(); it != value->end(); it++)
      cout << it->toString() << endl;
  } else if constexpr (std::is_same_v<T, dict_t>) {
    // for (auto it = value->begin(); it != value->end(); it++)
    //   cout << "\"" << it->first <<  "\":" << it->second << endl;
    for (auto &[k, v] : *value)
      cout << k << ":" << v.toString() << endl;
  } else {
    // unreachable
    assert(0);
  }
}

int main() {
  JsonParser parser;
  // std::string json_data = "{\"File\": null, \"Line\": 1, \"Money\": 2.2,
  // \"Message\": \"Hello!\"}";
  std::string json_data = "[1.2]";
  cout << json_data << endl;
  auto obj = parser.fromJson(std::move(json_data));

  // auto value = obj.getValuePtr<dict_t>();
  // print<dict_t>(value);
  auto value = obj.getValuePtr<list_t>();
  print<list_t>(value);
  // cout << obj.ToString() << endl;
  return 0;
}
