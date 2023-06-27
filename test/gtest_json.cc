#include <gtest/gtest.h>

#include <iostream>
#include <map>
#include <string>

#include "../jsonparser.hh"

using namespace json;
using namespace std;

template <typename T> string print(T *const value) {
  stringstream ss;
  if constexpr (is_same_v<T, null_t>) {
    ss << (value ? "?" : "nullptr");
  } else if constexpr (is_same_v<T, str_t>) {
    ss << *value;
  } else if constexpr (is_same_v<T, int_t>) {
    ss << *value;
  } else if constexpr (is_same_v<T, double_t>) {
    ss << *value;
  } else if constexpr (is_same_v<T, list_t>) {
    for (auto it = value->begin(); it != value->end(); it++)
      if (it + 1 != value->end())
        ss << it->toString() << ",";
      else
        ss << it->toString();
  } else if constexpr (is_same_v<T, dict_t>) {
    for (auto &[k, v] : *value)
      ss << k << ":" << v.toString() << '\n';
  } else {
    // unreachable
    assert(0);
  }
  return ss.str();
}

JsonParser parser;
TEST(tjson, normal_type) {
  std::string nt_jd = "1.2";
  auto obj = parser.fromJson(std::move(nt_jd));
  auto value = obj.getValuePtr<double_t>();

  EXPECT_EQ(*value, 1.2);
  EXPECT_EQ(obj.toString(), "1.200000");
}

TEST(tjson, dict) {
  std::string dict_jd = "{\"File\": null, \"Line\": 0}";
  auto obj = parser.fromJson(std::move(dict_jd));
  // auto value = obj.getValuePtr<dict_t>();

  // EXPECT_EQ(print<dict_t>(value), "\"File\":null\n\"Line\":0\n");
  EXPECT_EQ(obj.toString(), "{\"File\":null,\"Line\":0}");
}

TEST(tjson, nested) {
  std::string json_data = "[\"a\", [1, null]]";
  auto obj = parser.fromJson(std::move(json_data));

  EXPECT_EQ(obj.toString(), "[\"a\",[1,null]]");
}

TEST(tjson, nested2) {
  std::string json_data_nested2 = "{\"a\":[1, 2], \"b\":[2, {\"2\":null}]}";
  auto obj_nested2 = parser.fromJson(std::move(json_data_nested2));

  EXPECT_EQ(obj_nested2.toString(), "{\"a\":[1,2],\"b\":[2,{\"2\":null}]}");
}

TEST(tjson, nested3) {
  std::string nested3_jd = "{\"a\":[1, 2], \"b\":[2, {\"2\":null}]}";
  auto obj = parser.fromJson(std::move(nested3_jd));
  auto val = obj.getValuePtr<dict_t>();
  // auto val2 = obj.getValue<dict_t>();

  int i = 0;
  for (const auto &[k, v] : *val) {
    if (i == 0) {
      EXPECT_EQ(k, "\"a\"");
      // EXPECT_EQ(v.toString(), "[1,2]");
    } else if (i == 1) {
      EXPECT_EQ(k, "\"b\"");
      // EXPECT_EQ(v.toString(), "[2,{\"2\":null}]");
    }
    i++;
  }
  print<dict_t>(val);
  // EXPECT_EQ(obj.toString(), "{\"a\":[1,2],\"b\":[2,{\"2\":null}]}");
}

TEST(tjson, str) {
  string str_jd = "\'this is a string.\'";
  auto str_obj = parser.fromJson(std::move(str_jd));
  auto val = *str_obj.getValuePtr<str_t>();

  EXPECT_EQ(val, "this is a string.");
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
