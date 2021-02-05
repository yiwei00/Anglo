#include "AST.h"
#include "mem.h"
#include "interpreter.h"
#include <iostream>


using namespace std;

void example_AST();
void test_mem_stack();
void test_var_n_func();
void test_fun_copy();

int main(void) {
  try {
    test_var_n_func();
  } catch (char const* s) {
    cout << s << endl;
  }
  return 0;
}

void test_mem_stack() {
  MemStack *my_mem = new MemStack();
  bool ret = my_mem->replace("var1", Literal(42.0));
  if (!ret) cout << "passed test1" << endl;
  ret = my_mem->replace("var1", Literal(42.0)); //idempotency test
  if (!ret) cout << "passed test2" << endl;
  
  ret = my_mem->insert("var1", Literal(42.0));
  if (ret) cout << "passed test3" << endl;
  ret = my_mem->insert("var1", Literal(20.0));
  if (!ret) cout << "passed test4" << endl;

  my_mem->insert("var2", Literal(false));
  my_mem->insert("var3", Literal(string("hello there")));

  auto my_dat = my_mem->get_data("var1");
  if (my_dat->literal_type == _number && my_dat->number == 42.0)
    cout << "passed test5" << endl;
  my_mem->replace("var2", true);
  my_dat = my_mem->get_data("var2");
  if (my_dat->literal_type == _bool && my_dat->boolean)
    cout << "passed test6" << endl;
  
  int freed = my_mem->free_n(2);
  if (freed == 2) cout << "passed test7" << endl;

  try{
    my_mem->get_data("var3");
  } catch (const char *msg) {
    if (string(msg) == "variable doesn't exist")
      cout << "passed test8" << endl;
  }
  try {
    my_dat = my_mem->get_data("var1");
    if (my_dat->literal_type == _number && my_dat->number == 42.0)
      cout << "passed test9" << endl;
  } catch (const char *msg) {
    cout << "failed test9" << endl;
  }
  delete my_mem;
}

void example_AST() {
  // test cases for AST written out in python bc brain is actually garbage

  // def my_func(var1, var2):
  //     if var1:
  //         return None
  //     else:
  //         var2 = 3.4
  //         while true:
  //           print(var2)
  //         return "hello, world"
  // }

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
  delete my_func_0;
}

void test_var_n_func() {
  // test case written out in python:
  // 
  // def func(num):
  //     my_var = num
  //     return my_var
  //     my_var = 3 # this line shouldn't run
  //     return 3;
  // return func(69)
  std::vector<const Code *> code_block = {
    new Oper(_assign, {new Variable(string("func")), new Literal(new Function({new Variable(string("num"))},{
      new Oper(_assign, {new Variable(string("my_var")), new Variable(string("num"))}),
      new Return_Stmt(new Variable(string("my_var"))),
      new Oper(_assign, {new Variable(string("my_var")), new Literal(3.0)}),
      new Return_Stmt(new Literal(3.0))
    }))}),
    new Return_Stmt(new Func_Call(string("func"), {new Literal(69.0)}))
  };
  Interpreter *inter = new Interpreter();
  inter->exec_block(code_block);
  auto ret_val = inter->get_ret();
  if (ret_val->literal_type == _number) {
    if (ret_val->number == 69){
      cout << "test passed" << endl;
    }else {
      cout << "test failed :(" << endl;
    }
  } else {
    cout << "test failed :(" << endl;
  }
  
  for (auto p : code_block) delete p;
  delete inter;
}

void test_fun_copy() {
  Function *my_func_0 = new Function({new Variable("var1"), new Variable("var2")}, {
    new If_Stmt(new Variable("var1"), { //if the condition in var 1 is true
      new Return_Stmt(new Literal())
    }, { //else block
      new Oper(_assign, {new Variable("var2"), new Literal(3.4)}),
      new While_Stmt(new Literal(true), {
        new Func_Call("print", {new Variable("var2")})
      }),
      new Return_Stmt(new Literal("hello, world"))
    })
  });
  auto my_func_1 = new Function(*my_func_0);
  delete my_func_0;
  delete my_func_1;
}