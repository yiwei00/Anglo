#ifndef INTERPERTER_H
#define INTERPRETER_H

#include "mem.h"
#include "AST.h"
#include <string>
#include <vector>

class Interpreter {
public:
  Interpreter();
  ~Interpreter();

  void exec_block(const std::vector<const Code *> &cblock);
  void exec_code(const Code *c);
  inline Literal const *get_ret() { return ret_val; };
private:
  MemStack memory_stack;
  int scope_id;
  std::vector<int> var_count;
  const Literal *ret_val;
  const Literal *const null_literal;

  const Literal* eval_oper(const Oper *oper);
  const Literal* eval_expr(const Expression *expr);
  void exec_stmt(const Statement *stmt);
  void set_ret_val(const Literal &lit);

  //helpers or wrappers for certain operators
  void var_assign(const Variable &var, const Literal &val);

  //cannot be copied
  Interpreter(const Interpreter &other);
  Interpreter& operator=(const Interpreter &rhs);
};

#endif