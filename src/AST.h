#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

//forward declare block
class Variable;

enum _code_type {
  _expression,
  _statement,
};

class Code {
public:
  _code_type code_type;

  virtual ~Code();
  virtual Code* copy() = 0;
};

enum _expr_type {
  _literal,
  _operation,
  _variable,
};

class Expression : public Code {
public:
  _expr_type expr_type;

  inline Expression() { code_type = _expression; }
};

enum _literal_type {
  _number,
  _bool,
  _none,
  _string,
  _func,
};

class Function {
public:
  std::vector<Variable *> params;
  std::vector<Code *> body;

  Function(std::vector<Variable *> params, std::vector<Code *> body);
  ~Function();
  inline Function(const Function &other) { this->hard_copy(other); };
  Function& operator=(const Function &rhs);
private:
  //function called by the copy constructor and operator=
  void hard_copy(const Function &other);
};

class Literal : public Expression {
public:
  _literal_type literal_type;
  union {
    double number;
    bool boolean;
    void* none;
    std::string* str;
    Function* func;
  };

  Literal();
  Literal(double number);
  Literal(bool boolean);
  Literal(const std::string &str);
  Literal(Function *fun);

  ~Literal();
  inline Literal(const Literal &other) { this->hard_copy(other); };
  Literal& operator=(const Literal &rhs);

  inline Code* copy() { return new Literal(*this); };
private:
  //the function that copy, the copy constructor, and op=
  void hard_copy(const Literal &other);
};

class Variable : public Expression {
public:
  std::string var_name;

  Variable(const std::string &var_name);
  inline Variable* copy() { return new Variable(this->var_name); };
};

enum _oper_type {
  _add,
  _sub,
  _mul,
  _div,
  _assign,
  _concat,
  _equals,
  _logic_and,
  _logic_or,
};

class Oper : public Expression {
public:
  _oper_type oper_type;
  std::vector<Expression *> operands;

  Oper(_oper_type o_type, std::vector<Expression *> operands);
  ~Oper();

  inline Oper(const Oper &other) { this->hard_copy(other); };
  Oper& operator=(const Oper &rhs);
  
  inline Oper* copy() { return new Oper(*this); };

private:
  void hard_copy(const Oper &other);
};

enum _stmt_type {
  _return,
  _flow_if,
  _loop_while,
};

class Statement : public Code {
public:
  _stmt_type stmt_type;

  inline Statement() { code_type = _statement; }
};

class Return_Stmt : public Statement {
public:
  Expression *value;

  Return_Stmt(Expression *value);

  inline ~Return_Stmt() { delete value; };
  inline Return_Stmt(const Return_Stmt &other) { this->hard_copy(other); };
  Return_Stmt& operator=(const Return_Stmt &rhs);

  inline Return_Stmt* copy() { return new Return_Stmt(*this); };
private:
  void hard_copy(const Return_Stmt &other);
};

class If_Stmt : public Statement {
public:
  Expression *cond;
  std::vector<Code *> if_block;
  std::vector<Code *> else_block;

  If_Stmt(
    Expression *cond, 
    std::vector<Code *> if_block, 
    std::vector<Code *> else_block
  );

  ~If_Stmt();
  inline If_Stmt(const If_Stmt &other) { this->hard_copy(other); };
  If_Stmt& operator=(const If_Stmt &rhs);

  inline If_Stmt* copy() { return new If_Stmt(*this); };
private:
  void hard_copy(const If_Stmt &other);
};

class While_Stmt : public Statement {
public:
  Expression *cond;
  std::vector<Code *> body;

  ~While_Stmt();
  inline While_Stmt(const While_Stmt &other) { this->hard_copy(other); };
  While_Stmt& operator=(const While_Stmt &rhs);

  inline While_Stmt* copy() { return new While_Stmt(*this); };
private:
  void hard_copy(const While_Stmt &other);
};

#endif