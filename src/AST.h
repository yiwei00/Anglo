/*
written by Yi Wei in January 2021
contains the definition for the AST data structure
*/
#ifndef AST_H
#include <vector>
#include <string>

//forward declare
struct Expression;
struct Statement;

typedef enum {
  _Expr,
  _State
} _code_type;

struct Code {
  _code_type code_type;
  union {
    Expression* expr;
    Statement* state;
  };
  ~Code();
};

typedef enum {
  _Number,
  _Bool,
  _None,
  _String,
  _Func,
} _literal_type;

struct Literal {
public:
  _literal_type type;
  union {
    double number;
    bool boolean;
    void *none;
    std::string *str;
    void *func;
  };

  Literal(double num);
  Literal(bool boolean);
  Literal(const std::string &str);
  //rule of three since we have destructor
  Literal(const Literal &other);
  Literal& operator=(const Literal &rhs);

  ~Literal();
private:
  void swap(const Literal &rhs);
};

typedef enum {
  _Liter,
  _Oper,
  _Var,
} _expr_type;

struct Expression {
  virtual ~Expression() = 0; //force abstraction via destructor
  _expr_type expr_type;
};

typedef enum {
  _Add,
  _Sub,
  _Mul,
  _Div,
  _Assign,
  _Concat,
  _Equals,
  _Logic_And,
  _Logic_Or,
} _oper_type;

struct Oper_Expr : public Expression {
  _oper_type oper_type;
  std::vector<Expression *> operands;

  Oper_Expr(_oper_type oper_type, std::vector<Expression *> operands);
  ~Oper_Expr();

private:
  Oper_Expr(const Oper_Expr &other);
  Oper_Expr& operator=(const Oper_Expr &rhs);
  
};

struct Var_Expr : public Expression {
  std::string var_name;

  Var_Expr(const std::string &var_name);
};

struct Lit_Expr : public Expression {
  Literal liter;

  Lit_Expr(const Literal &liter);
};

typedef enum {
  cond_if,
  loop_while,
} _state_type;

struct Statement {
  _state_type state_type;
  virtual ~Statement() = 0; //force abstraction bc complicated
};

struct If_State : public Statement {
  Expression* cond;
  std::vector<Code *> true_block;
  std::vector<Code *> false_block;

  If_State(Expression *cond, std::vector<Code *> true_block, std::vector<Code *> false_block):
    cond(cond), 
    true_block(true_block), 
    false_block(false_block)
    {};
  ~If_State();
private:
  If_State(const If_State &other);
  If_State& operator=(const If_State &rhs);
};

struct While_State : public Statement {
  Expression* cond;
  std::vector<Code *> loop_block;
  While_State(Expression *cond, std::vector<Code *> loop_block):
    cond(cond),
    loop_block(loop_block)
    {};
  ~While_State();
private:
  While_State(const While_State &other);
  While_State& operator=(const While_State &rhs);
};

#endif