#include "AST.h"
#include <iostream>//TODO: remove this is for testing purpose

using namespace std;

Code::~Code() {
  if (code_type == _Expr)
    delete expr;
  else
    delete state;
}

/*
All memories in params and body will now be handled by this struct
*/
Function::Function(std::vector<Var_Expr *> params, std::vector<Code *> body) {
  this->params = params;
  this->body = body;
}

Function::~Function() {
  if (swapped) return;
  for (auto p_ptr : params) {
    delete p_ptr;
  }
  for (auto c_ptr: body) {
    delete c_ptr;
  }
}

Literal::Literal(double num) {
  type = _Number;
  number = num;
}

Literal::Literal(bool boolean) {
  type = _Bool;
  this->boolean = boolean;
}

Literal::Literal(const std::string &str) {
  type = _String;
  this->str = new string(str);
}

Literal::Literal(const Literal& other) {
  this->type = other.type;
  switch (type) {
  case _Number: 
    this->number = other.number;
    break;
  case _Bool:
    this->boolean = other.boolean;
    break;
  case _None:
    this->none = nullptr;
    break;
  case _String:
    this->str = new string(*(other.str));
    break;
  case _Func:
    //TODO: functions
    break;
  default:
    break;
  }
}

Literal& Literal::operator=(const Literal &rhs) {
  if (type == _String)
    delete str;
  type = rhs.type;
  switch(type) {
    case _Number: 
    this->number = rhs.number;
    break;
  case _Bool:
    this->boolean = rhs.boolean;
    break;
  case _None:
    this->none = nullptr;
    break;
  case _String:
    this->str = new string(*(rhs.str));
    break;
  case _Func:
    this->func = rhs.func;
    break;
  default:
    break;
  }
  return *this;
}

Literal::~Literal() {
  if (type == _String)
    delete str;
}

Expression::~Expression() {};

Oper_Expr::Oper_Expr(_oper_type oper_type, vector<Expression *> operands) {
  this->oper_type = oper_type;
  this->operands = operands;
  this->expr_type = _Oper;
}

Oper_Expr::~Oper_Expr() {
  for (Expression *expr_ptr : operands) {
    delete expr_ptr;
  }
}

Var_Expr::Var_Expr(const string &var_name) {
  this->var_name = var_name;
  this->expr_type = _Var;
}

Lit_Expr::Lit_Expr(const Literal &liter):
liter(liter) {
  this->expr_type = _Liter;
}

Statement::~Statement(){};

If_State::If_State(Expression *cond, std::vector<Code *> true_block, std::vector<Code *> false_block):
cond(cond), 
true_block(true_block), 
false_block(false_block) {
  this->state_type = _cond_if;
};

If_State::~If_State() {
  delete cond;
  for (Code *code_ptr : true_block)
    delete code_ptr;
  for (Code *code_ptr : false_block)
    delete code_ptr;
}

While_State::While_State(Expression *cond, std::vector<Code *> loop_block) :
cond(cond),
loop_block(loop_block){
  this->state_type = _loop_while;
}

While_State::~While_State() {
  delete cond;
  for (Code *code_ptr : loop_block)
    delete code_ptr;
}