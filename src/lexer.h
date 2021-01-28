#ifndef LEXER_H
#include <string>

enum _token_type {
  _symbol,
  _punctuation,
  _string,
  _number,
  _unknown,
};

class Token {
public:
  Token(_token_type type, const std::string &token);
  _token_type get_type() { return type; }
  const std::string& get_tok() { return token; }
private:
  _token_type type;
  std::string token;
};

class Lexer {
public:
  Lexer(const std::string &buf);
  Token* next_tok();
private:
  const std::string &buf;
  std::string::const_iterator cur;
};

#endif