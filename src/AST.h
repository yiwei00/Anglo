/*
written by Yi Wei in January 2021
contains the definition for the AST data structure
*/
#ifndef AST_H
#include <vector>
#include <string>

//forward declarations
struct Expression;
struct Statement;
struct Oper_Expr;
struct Lit_Expr;
struct Var_Expr;
struct If_State;
struct While_State;

enum _code_type{
  _Expr,
  _State
};

struct Code {
  _code_type code_type;
  union {
    Expression* expr;
    Statement* state;
  };
  ~Code();
};

enum _expr_type{
  _Liter,
  _Oper,
  _Var,
};

struct Expression {
  virtual ~Expression() = 0; //force abstraction via destructor
  _expr_type expr_type;
};

enum _literal_type{
  _Number,
  _Bool,
  _None,
  _String,
  _Func,
};

struct Function {
  std::vector<Var_Expr *> params;
  std::vector<Code *> body;
  
  Function(std::vector<Var_Expr *> params, std::vector<Code *> body);
  
  ~Function();

private:
  bool swapped = false;

  Function& swap(Function &other);
  Function(const Function &other);
  Function& operator=(const Function &rhs);
};

struct Literal {
public:
  _literal_type type;
  union {
    double number;
    bool boolean;
    void *none;
    std::string *str;
    Function *func;
  };

  Literal(double num);
  Literal(bool boolean);
  Literal(const std::string &str);
  //rule of three since we have destructor
  Literal(const Literal &other);
  Literal& operator=(const Literal &rhs);

  ~Literal();
};

struct Lit_Expr : public Expression {
  Literal liter;

  Lit_Expr(const Literal &liter);
};

enum _oper_type{
  _Add,
  _Sub,
  _Mul,
  _Div,
  _Assign,
  _Concat,
  _Equals,
  _Logic_And,
  _Logic_Or,
};

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

enum _state_type{
  _cond_if,
  _loop_while,
};

struct Statement {
  _state_type state_type;
  virtual ~Statement() = 0; //force abstraction bc complicated
};

struct If_State : public Statement {
  Expression* cond;
  std::vector<Code *> true_block;
  std::vector<Code *> false_block;

  If_State(Expression *cond, std::vector<Code *> true_block, std::vector<Code *> false_block);
  ~If_State();
private:
  If_State(const If_State &other);
  If_State& operator=(const If_State &rhs);
};

struct While_State : public Statement {
  Expression* cond;
  std::vector<Code *> loop_block;
  While_State(Expression *cond, std::vector<Code *> loop_block);
  ~While_State();
private:
  While_State(const While_State &other);
  While_State& operator=(const While_State &rhs);
};

#endif