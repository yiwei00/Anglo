#ifndef LEXER_H
#include <string>

typedef enum {
  _symbol,
  _punctuation,
  _string,
  _number,
  _unknown,
} token_type;

class Token {
public:
  Token(token_type type, const std::string &token);
  token_type get_type() { return type; }
  const std::string& get_tok() { return token; }
private:
  token_type type;
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