#include "lexer.h"
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>

using namespace std;

//helper ctype style function, customized for my purpose
static bool is_op(char c) {
  const string op_char = "=><+-/*";
  return (op_char.find(c) != string::npos);
}

static bool is_digit(char c) {
  return (c >= '0' && c <= '9');
}

static bool is_term(char c) {
  const string terms = "().,;";
  return (terms.find(c) != string::npos);
}

static bool is_keyword(string s) {
  vector<string> keyword_set = {
    "TASK",
    "USES",
    "SO",
    "IF",
    "THEN",
    "OTHERWISE",
    "PRODUCE"
  };
  return find(keyword_set.begin(), keyword_set.end(), s) != keyword_set.end();
}
static bool is_op_word(string s) {
  vector<string> op_word_set = {
    "IS",
    "AND",
    "OR"
  };
  return find(op_word_set.begin(), op_word_set.end(), s) != op_word_set.end();
}

Token::Token(_token_type type, const string &token) {
  this->type = type;
  this->token = string(token);
}

Lexer::Lexer(const string &buf): 
buf(buf) {
  cur = this->buf.begin();
}

Token* Lexer::next_tok() {
  if (cur == buf.end())
    return nullptr;
  while (isspace(*cur)) { //skips white space space
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
    return new Token(_str, string(tok_start+1, cur++));//the ++ afterwards so cur is correctly set
  } 
  else if (is_digit(*cur)) { //numbers
    while (is_digit(*cur)) {
      ++cur;
      if (cur == buf.end())
        return new Token(_num, string(tok_start, cur));
    }
    if ((*cur) != '.')
      return new Token(_num, string(tok_start, cur));
    ++cur;
    if (cur == buf.end())
      return new Token(_num, string(tok_start, cur));
    while (is_digit(*cur)) {
      ++cur;
      if (cur == buf.end())
        return new Token(_num, string(tok_start, cur));
    }
    return new Token(_num, string(tok_start, cur));
  } 
  else if (isalpha(*cur)) { //identifier or possible keywords/op words
    while ((*cur == '_') || isalnum(*cur) ){
      ++cur;
      if (cur == buf.end())
        break;
    }
    string s = string(tok_start, cur);
    if (is_keyword(s)) {
      return new Token(_keyword, s);
    }
    else if (is_op_word(s)) {
      return new Token(_op, s);
    }
    else
      return new Token(_id, s);
  }
  else if (is_op(*cur)) { //operator
    while (is_op(*cur)) {
      ++cur;
      if (cur == buf.end())
        break;
    }
    return new Token(_op, string(tok_start, cur));
  }
  else if (is_term(*cur)) { //terminal
    return new Token(_term, string(tok_start, ++cur));
  }
  //TODO: throw error instead of return null
  return nullptr;
}