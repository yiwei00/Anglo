#include <iostream>
#include "lexer.h"

using namespace std;

int main(void)
{
  string input = "let my_str = 'hello, world'().1234\n";
  Lexer luther = Lexer(input);
  Token *tok = nullptr;
  while ((tok = luther.next_tok()) != nullptr) {
    cout << "token \"" << tok->get_tok() << "\" of type " << tok->get_type() << endl;
  }
  return 0;
}