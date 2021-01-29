#include "AST.h"
#include <iostream>
#include <vector>

/* 
test cases for AST written out in python bc brain is actually garbage

def my_func(var1, var2):
    if var1:
        return None
    else:
        var2 = 3.4
        while true:
          print(var2)
        return "hello, world"
}

*/

using namespace std;

int main(void) {
  Code* my_func_0 = 
  new Oper(_assign, {
    new Variable("my_func"),
    new Literal(new Function({new Variable("var1"), new Variable("var2")}, {
      new If_Stmt(new Variable("var1"), { //if the condition in var 1 is true
        new Return_Stmt(new Literal())
      }, { //else block
        new Oper(_assign, {new Variable("var2"), new Literal(3.4)}),
        new While_Stmt(new Literal(true), {
          new Func_Call("print", {new Variable("var2")})
        }),
        new Return_Stmt(new Literal("hello, world"))
      })
    }))
  }
  );

  Code* my_func = my_func_0->copy();
  delete my_func_0;


  if (my_func->code_type == _expression)
    cout << "passed t1" << endl;
  else 
    cout << "failed t1" << endl;

  auto my_def = (Oper *) my_func;
  if (my_def->operands[1]->expr_type == _literal)
    cout << "passed t2" << endl;
  else 
    cout << "failed t2" << endl;
  
  auto fun_lit = ((Literal *) my_def->operands[1])->func;
  if ((fun_lit->body[0])->code_type == _statement) {
    auto fun_stat = (Statement *) fun_lit->body[0];
    if (fun_stat->stmt_type == _flow_if)
      cout << "passed t3" << endl;
    else
      cout << "failed t3" << endl;
  }

  return 0;
}