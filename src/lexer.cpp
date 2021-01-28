#include "lexer.h"
#include <string>

using namespace std; //the full namespace std has some conflicts

const static string puncts = "(),.-=\n";

//helper cctype style function, customized for my purpose

static bool is_space(char c) {
  return (c == ' ' || c == '\t' || c == '\014');
}

static bool is_symb(char c) {
  return ((c >= 'A' && c <= 'Z') ||
          (c >= 'a' && c <= 'z') ||
          (c == '_'));
}

static bool is_digit(char c) {
  return (c >= '0' && c <= '9');
}

static bool is_punct(char c) {
  return (puncts.find(c) != string::npos);
}

Token::Token(_token_type type, const string &token) {
  this->type = type;
  this->token = string(token);
}

Lexer::Lexer(const string &buf): 
buf(buf)
{
  cur = buf.begin();
}

Token* Lexer::next_tok() {
  if (cur == buf.end())
    return nullptr;
  while (is_space(*cur)) { //skips white space space
    ++cur;
    if (cur == buf.end())
      return nullptr;
  }
  
  auto tok_start = cur;
  if (*cur == '\'' || *cur == '"') { //string
    //naive string implementation, does not support escape characters or more
    do {
      ++cur;
      if (cur == buf.end())
        break;
    } while (*cur != *tok_start);
    return new Token(_string, string(tok_start+1, cur++));//the ++ so cur is correctly set
  } else if (is_digit(*cur)) {
    while (is_digit(*cur)) {
      ++cur;
      if (cur == buf.end())
        break;
    }
    return new Token(_number, string(tok_start, cur));
  } else if (is_symb(*cur)) {
    while (is_symb(*cur)){
      ++cur;
      if (cur == buf.end())
        break;
    }
    return new Token(_symbol, string(tok_start, cur));
  } else if (is_punct(*cur)) {
    return new Token(_punctuation, string(tok_start, ++cur));
  }
  //TODO: throw error instead of return null
  return nullptr;
}