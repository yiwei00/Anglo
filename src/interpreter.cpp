#include "interpreter.h"

Interpreter::Interpreter()
: null_literal(new Literal()) {
  scope_id = 0;
  var_count = {0};
  ret_val = new Literal();
}

Interpreter::~Interpreter() {
  delete null_literal;
  delete ret_val;
}

void Interpreter::exec_block(const std::vector<const Code *> &cblock) {
  ret_val = null_literal;
  int init_sid = scope_id;
  for (size_t i = 0; i < cblock.size() && scope_id >= init_sid; ++i) {
    exec_code(cblock[i]);
  }
}

void Interpreter::exec_code(const Code *c) {
  switch (c->code_type) {
  case _expression:
    eval_expr(((Expression *) c));
    break;
  case _statement:
    exec_stmt(((Statement *) c));
    break;
  default:
    break;
  }
}

const Literal* Interpreter::eval_oper(const Oper *oper) {
  const Literal* res = nullptr;
  switch (oper->oper_type) {
  case _assign: {
    if (oper->operands[0]->expr_type != _variable)
      throw "invalid operand type";
    res = eval_expr(oper->operands[1]);
    var_assign(*((Variable *) oper->operands[0]), *res);
    break;
  }
  default:
    throw "unimplemented oper";
    break;
  }
  return res;
}

const Literal* Interpreter::eval_expr(const Expression *expr) {
  const Literal *res = null_literal;
  switch (expr->expr_type) {
  case _literal: {
    res = (const Literal *) expr;
    break;
  }
  //handle operation
  case _operation: {
    auto oper = (const Oper *) expr;
    res = eval_oper(oper);
    break;
  }
  //handle variable accessing
  case _variable: {
    auto var = (const Variable *) expr;
    try {
      res = memory_stack.get_data(var->var_name);
    } catch (const char *msg) {
      throw msg; //up goes the exception!
    }
    break;
  }
  //handle function calls, simulates call stack
  case _func_call: {
    const Func_Call *fun_call = (const Func_Call *) expr;
    Function *fun;
    try {
      const Literal *fun_lit = memory_stack.get_data(fun_call->func_name);
      if (fun_lit->literal_type != _func) //make sure it's executable
        throw "cannot call non function variable";
      fun = fun_lit->func;
    } catch (const char *msg) {
      throw msg; //up goes the exception!
    }
    int cur_scope = ++scope_id;
    //extend var_count if necessary
    int diff = (scope_id + 1) - var_count.size();
    while (diff > 0) {
      var_count.push_back(0);
      --diff;
    }
    var_count[scope_id] = 0; //reset var_count for new scope
    //assign args to params
    for (size_t i = 0; i < fun->params.size(); ++i) {
      var_assign(*(fun->params[i]), *eval_expr(fun_call->args[i]));
    }
    //clear ret_val;
    ret_val = null_literal;
    //run func
    for(auto c : fun->body){
      exec_code(c);
      if (scope_id != cur_scope){
        if (scope_id == (cur_scope - 1)) break;
        else throw "stack error";
      }
    }
    //cleanup
    int freed = memory_stack.free_n(var_count[cur_scope]);
    if (freed != var_count[cur_scope])
      throw "incorrect memory freed from callstack";
    res = ret_val;
    break;
  }
  default:
    throw "unknown expression type encountered";
    break;
  }
  return res;
}

void Interpreter::exec_stmt(const Statement *stmt) {
  switch(stmt->stmt_type) {
  case _return: {
    auto ret = (const Return_Stmt *) stmt;
    ret_val = eval_expr(ret->value);
    --scope_id;
    break;
  }
  default:
    throw "stmt feature unimplemented";
    break;
  }
}

void Interpreter::set_ret_val(const Literal &lit) {
  if (ret_val != &lit) {
    delete ret_val;
    ret_val = lit.copy();
  }
}

void Interpreter::var_assign(const Variable &var, const Literal &val) {
  if (!memory_stack.replace(var.var_name, val)) {
    ++(var_count[scope_id]);
    memory_stack.insert(var.var_name, val);
  }
}