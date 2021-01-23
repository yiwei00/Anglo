#include "AST.h"
#include <iostream>
#include <vector>


using namespace std;

int main(void)
{
  /* string input = "let my_str = 'hello, world'().1234\n";
  Lexer luther = Lexer(input);
  Token *tok = nullptr;
  while ((tok = luther.next_tok()) != nullptr) {
    cout << "token \"" << tok->get_tok() << "\" of type " << tok->get_type() << endl;
  } */
  vector<Code *> code_block;
  auto code = new Code();
  code->code_type = _Expr;
  auto inner_oper = new Oper_Expr(_Sub, vector<Expression *>({new Lit_Expr(Literal(3.4)), new Lit_Expr(Literal(4.5))}));
  auto outter_oper = new Oper_Expr(_Mul, {new Var_Expr("myVar"), inner_oper});
  auto cond = new Oper_Expr(_Logic_And, {new Lit_Expr(Literal(true)), new Lit_Expr(Literal(false))});
  code->expr = outter_oper;
  auto loop_block = vector<Code *>({code});
  While_State *test_loop = new While_State(cond, loop_block);
  delete test_loop;
  cout << "wow" << endl;
  return 0;
}