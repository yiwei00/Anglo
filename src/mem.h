#ifndef MEM_H
#define MEM_H

#include "AST.h"
#include <unordered_map> //the hash slinging SLASHER!
#include <stack>

class MemStack{
public:
  MemStack():mem_store(), mem_stack() {};
  ~MemStack();

  Literal* get_data(const std::string &var_name); //retriees data, if not found return nullptr
  bool replace(const std::string &var_name, const Literal &value); //replace if data is found, otherwise return false
  bool insert(const std::string &var_name, const Literal &value); //add new mem to stack, if exists return false
  int free_n(int n); //frees the last n variables returns however many freed
private:
  //stores the actual mapping from var name to memory
  std::unordered_map<std::string, Literal *> mem_store;
  //simulates the stack
  std::stack<std::string> mem_stack;
  //copying not allowed, keep private no need to implement
  MemStack(const MemStack &other);
  const MemStack& operator=(const MemStack &rhs);
};

#endif