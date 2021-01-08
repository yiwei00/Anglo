#include <vector>
#include <string>

typedef enum {
  whitespace,
  alphanum,
  punctuation,
  unknown
} char_type;

char_type get_type(const char &c);

struct Token{
  std::string tok;
  char_type c;
  Token(std::string tok) :
    tok(tok),
    c(get_type(tok[0]))
    {};
};

std::vector<Token> tokenize(const std::string &token_string);