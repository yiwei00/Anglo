#include "lexer.h"
#include <cctype>

using namespace std;

string special_char = "+-*/%,;.=|&><(){}[]";

char_type get_type(const char &c) {
  if (isspace(c) && c != '\n')
    return whitespace;
  if (isalnum(c))
    return alphanum;
  string punct_list = "!@#$%^&*()_+-=`~[]\\;',./{}|:\"<>?\n";
  if (punct_list.find(c) != string::npos)
    return punctuation;
  return unknown;
}

vector<Token> tokenize(const string &token_string) {
  vector<Token> tokens;
  char_type start_type = get_type(*token_string.begin());
  for (auto tok_start = token_string.begin(), i = tok_start; i <= token_string.end(); ++i) {
    if (i != token_string.end() && start_type == get_type(*i))
      continue;
    tokens.push_back(Token(string(tok_start, i)));
    tok_start = i;
    if (i != token_string.end())
      start_type = get_type(*i);
  }
  return tokens;
}