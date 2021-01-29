#include "AST.h"
#include <iostream> //TODO: remove iostream after testing

using namespace std;

Code::~Code() {};

Code* Code::copy() { return nullptr; };

Function::Function(std::vector<Variable *> params, std::vector<Code *> body) {
  this->params = params;
  this->body = body;
}

Function::~Function() {
  for (Variable *p_ptr : params) {
    delete p_ptr;
  }
  for (Code *c_ptr : body) {
    delete c_ptr;
  }
}

Function& Function::operator=(const Function &rhs) {
  for (Variable *p_ptr : params) {
    delete p_ptr;
  }
  for (Code *c_ptr : body) {
    delete c_ptr;
  }

  this->hard_copy(rhs);

  return *this;
}

void Function::hard_copy(const Function &other) {
  for (auto p_ptr : other.params) {
    this->params.push_back(p_ptr->copy());
  }
  for (auto c_ptr : other.body) {
    this->body.push_back(c_ptr->copy());
  }
}

Literal::Literal()
: Expression() {
  expr_type = _literal;
  literal_type = _none;
  none = nullptr;
}

Literal::Literal(double number)
: Expression() {
  expr_type = _literal;
  literal_type = _number;
  this->number = number;
}

Literal::Literal(bool boolean)
: Expression() {
  expr_type = _literal;
  this->literal_type = _bool;
  this->boolean = boolean;
}

Literal::Literal(const string &str)
: Expression() {
  expr_type = _literal;
  literal_type = _string;
  this->str = new string(str);
}

Literal::Literal(Function *fun)
: Expression() {
  expr_type = _literal;
  literal_type = _func;
  this->func = fun;
}

Literal::~Literal() {
  switch (this->literal_type) {
  case _string:
    delete str;
    break;
  case _func:
    delete func;
    break;
  case _number: case _bool: case _none: default:
    break;
  }
}

Literal& Literal::operator=(const Literal &rhs) {
  switch (this->literal_type) {
  case _string:
    delete str;
    break;
  case _func:
    delete func;
    break;
  case _number: case _bool: case _none: default:
    break;
  }

  this->hard_copy(rhs);

  return *this;
}

void Literal::hard_copy(const Literal &other) {
  this->literal_type = other.literal_type;
  this->expr_type = other.expr_type;
  this->code_type = other.code_type;

  switch (this->literal_type) {
  case _number:
    this->number = other.number;
    break;
  case _bool:
    this->boolean = other.boolean;
    break;
  case _none:
    this->none = nullptr;
    break;
  case _string:
    this->str = new string(*(other.str));
    break;
  case _func:
    this->func = new Function(*(other.func));
    break;
  }
}

Variable::Variable(const string &var_name) 
: Expression() {
  expr_type = _variable;
  this->var_name = var_name;
}

Oper::Oper(_oper_type o_type, std::vector<Expression *> operands)
: Expression() {
  expr_type = _operation;
  oper_type = o_type;
  this->operands = operands;
}

Oper::~Oper() {
  for (auto e_ptr : operands) {
    delete e_ptr;
  }
}

Oper& Oper::operator=(const Oper &rhs) {
  for (auto e_ptr : this->operands) {
    delete e_ptr;
  }
  this->hard_copy(rhs);
  return *this;
}

void Oper::hard_copy(const Oper &other) {
  this->code_type = other.code_type;
  this->expr_type = other.expr_type;
  this->oper_type = other.oper_type;

  for (auto e_ptr : other.operands) {
    this->operands.push_back((Expression *) e_ptr->copy());
  }
}

Func_Call::Func_Call(const std::string &func_name, std::vector<Expression *> args)
:Expression() {
  expr_type = _func_call;
  this->func_name = func_name;
  this->args = args;
}

Func_Call::~Func_Call() {
  for (auto e_ptr : args)
    delete e_ptr;
}

Func_Call& Func_Call::operator=(const Func_Call &rhs) {
  for (auto e_ptr : args)
    delete e_ptr;
  this->hard_copy(rhs);
  return *this;
}

void Func_Call::hard_copy(const Func_Call &other) {
  this->code_type = other.code_type;
  this->expr_type = other.expr_type;

  for (auto e_ptr : other.args) {
    this->args.push_back((Expression *) e_ptr->copy());
  }
}

Return_Stmt::Return_Stmt(Expression *value)
: Statement() {
  this->value = value;
}

Return_Stmt& Return_Stmt::operator=(const Return_Stmt &rhs) {
  delete value;
  this->hard_copy(rhs);
  return *this;
}

void Return_Stmt::hard_copy(const Return_Stmt &other) {
  this->code_type = other.code_type;
  this->stmt_type = other.stmt_type;
  this->value = (Expression *) other.value->copy();
}

If_Stmt::If_Stmt(
  Expression *cond, 
  std::vector<Code *> if_block, 
  std::vector<Code *> else_block
): Statement() {
  stmt_type = _flow_if;
  
  this->cond = cond;
  this->if_block = if_block;
  this->else_block = else_block;
};

If_Stmt::~If_Stmt() {
  delete cond;
  for (auto e_ptr : if_block)
    delete e_ptr;
  for (auto e_ptr : else_block)
    delete e_ptr;
}

If_Stmt& If_Stmt::operator=(const If_Stmt &rhs) {
  delete cond;
  for (auto e_ptr : if_block)
    delete e_ptr;
  for (auto e_ptr : else_block)
    delete e_ptr;

  this->hard_copy(rhs);

  return *this;
}

void If_Stmt::hard_copy(const If_Stmt &other) {
  this->code_type = other.code_type;
  this->stmt_type = other.stmt_type;
  this->cond = (Expression *) other.cond->copy();
  for (auto e_ptr : other.if_block)
    this->if_block.push_back(e_ptr->copy());
  for (auto e_ptr : other.else_block)
    this->else_block.push_back(e_ptr->copy());
}

While_Stmt::While_Stmt(Expression *cond, std::vector<Code *> body)
: Statement() {
  stmt_type = _loop_while;

  this->cond = cond;
  this->body = body;
}

While_Stmt::~While_Stmt() {
  delete cond;
  for (auto e_ptr : body)
    delete e_ptr;
}

While_Stmt& While_Stmt::operator=(const While_Stmt &rhs) {
  delete cond;
  for (auto e_ptr : body)
    delete e_ptr;

  this->hard_copy(rhs);

  return *this;
}

void While_Stmt::hard_copy(const While_Stmt &other) {
  this->code_type = other.code_type;
  this->stmt_type = other.stmt_type;
  this->cond = (Expression *) other.cond->copy();
  for (auto e_ptr : other.body)
    this->body.push_back(e_ptr->copy());
}