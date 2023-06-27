#include "jsonparser.hh"

#include <cassert>
#include <cctype>
#include <sstream>
#include <stdexcept>

namespace json {

auto JsonParser::fromJson(std::string &&json_data) -> JsonObject {
  m_data = std::forward<std::string>(json_data);
  m_pos = 0;
  return JsonObject(parse());
}

auto JsonParser::toJson(JsonObject &&jsonobject) -> std::string {
  return jsonobject.toString();
}

auto JsonParser::serialize(JsonObject &&jsonobject) -> std::string {
  return jsonobject.toString();
}

auto JsonParser::get_next_token() -> char {
  size_t size = m_data.size();
  while (m_pos < size && isspace(m_data[m_pos])) {
    ++m_pos;
  }

  skip_comment();

  if (m_pos < size)
    return m_data[m_pos];

  // unreachable
  // not suitable json string
  throw std::runtime_error("invalid json string, while running get_next_token");
}

void JsonParser::skip_comment() {
  size_t size = m_data.size();
  if (m_pos + 1 < size && m_data[m_pos] == '/') {
    // '//'
    if (m_data[m_pos + 1] == '/')
      for (; m_pos < size && m_data[m_pos] != '\n'; m_pos++)
        ;
    if (m_pos >= size)
      // error fmt "//"
      throw std::runtime_error(
          "invalid comment. correct one is '/* */', while running "
          "skip_comment");
    else if (m_data[m_pos + 1] == '*') {
      for (; m_pos + 1 < size &&
             !(m_data[m_pos] == '*' && m_data[m_pos + 1] == '/');
           m_pos++)
        ;
      ++m_pos;
      if (m_pos >= size)
        // error fmt "/* */"
        throw std::runtime_error(
            "invalid comment. correct one is '/* */', while running "
            "skip_comment");
    }
  }

  // skip '\n' or '/'
  if (m_data[m_pos] == '\n' || m_data[m_pos] == '/')
    ++m_pos;

  if (m_pos >= size)
    throw std::runtime_error("invalid json string, while running skip_comment");
}

auto JsonParser::parse() -> JsonObject {
  char token = get_next_token();
  JsonObject obj{};
  switch (token) {
  // T_NULL
  case 'n':
  case 'N':
    obj = parse_null();
    break;
  // T_INT
  // T_DOUBLE
  case '+':
  case '-':
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    obj = parse_number();
    break;
  // T_STR
  case '\"':
  case '\'':
    obj = parse_string();
    break;
  // T_LIST
  case '[':
    obj = parse_list();
    break;
  // T_DICT
  case '{':
    obj = parse_dict();
    break;
  default:
    // unknown
    throw std::runtime_error("invalid token");
  }
  return obj;
}

auto JsonParser::parse_null() -> JsonObject {
  // can be removed
  char token = m_data[m_pos];
  assert(token == 'n' || token == 'N');

  std::string null = m_data.substr(m_pos, 4);
  for (size_t i = 0; i < null.size(); ++i)
    null[i] = tolower(null[i]);

  if (null != "null") {
    throw std::runtime_error("null is correct in parse_null");
  }

  m_pos += 4;
  return JsonObject();
}

auto JsonParser::parse_string() -> JsonObject {
  // can be removed
  char token = m_data[m_pos];
  assert(token == '\'' || token == '\"');

  char quota = m_data[m_pos++];
  size_t old_pos = m_pos;
  size_t len;

  for (len = 0; m_pos < m_data.size() && m_data[m_pos] != quota; ++len, ++m_pos)
    ;

  assert(m_pos < m_data.size());
  ++m_pos;

  return JsonObject(m_data.substr(old_pos, len));
}

auto JsonParser::parse_number() -> JsonObject {
  // can be removed
  char token = m_data[m_pos];
  assert(isdigit(token) || token == '-');

  const char *cstr = m_data.c_str() + m_pos;
  int num = atoi(cstr);

  // skip '-'
  if (m_data[m_pos] == '-')
    ++m_pos;

  for (; isdigit(m_data[m_pos]); ++m_pos)
    ;

  if (m_data[m_pos] == '.') {
    double dnum = atof(cstr);

    ++m_pos; // skip '.'
    // skip the remain part
    for (; isdigit(m_data[m_pos]); ++m_pos)
      ;
    return JsonObject(dnum);
  }

  return JsonObject(num);
}

auto JsonParser::parse_list() -> JsonObject {
  char token;
  // can be removed
  token = m_data[m_pos];
  assert(token == '[');

  list_t list{};
  ++m_pos; // NOTED
  token = get_next_token();
  while (token != ']') {
    list.emplace_back(parse());
    token = get_next_token();
    if (token == ']')
      break;
    if (token != ',') {
      // error list format
      throw std::runtime_error("error list format ','");
    }
    ++m_pos; // NOTED
    token = get_next_token();
  }

  m_pos++;
  return JsonObject(list);
}

auto JsonParser::parse_dict() -> JsonObject {
  char token;
  // can be removed
  token = m_data[m_pos];
  assert(token == '{');

  dict_t dict{};

  ++m_pos; // NOTED
  token = get_next_token();
  while (token != '}') {
    // key
    const std::string &str = parse_string().toString();
    token = get_next_token();
    if (token != ':') {
      // error dict format ':'
      throw std::runtime_error("invalid token when parsing dict ':'");
    }
    ++m_pos; // NOTED
    // value
    token = get_next_token();
    dict[str] = parse();
    // ,
    token = get_next_token();
    if (token == '}')
      break;
    if (token != ',') {
      // error dict format
      throw std::runtime_error("error dict format ','");
    }
    ++m_pos; // NOTED
    token = get_next_token();
  }

  m_pos++;
  return JsonObject(dict);
}

} // namespace json
