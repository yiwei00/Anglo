#include <iostream>
#include "lexer.h"

using namespace std;

int main(void)
{
  string input = "public static void main()";
  auto tokens = tokenize(input);
  for (std::vector<Token>::size_type i = 0; i < tokens.size(); ++i) {
    cout << tokens[i].tok << " of type: " << (int) tokens[i].c << endl;
  }
  return 0;
}