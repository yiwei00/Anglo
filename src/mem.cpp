  #include "mem.h"
  
  using namespace std;

  MemStack::~MemStack() {
    int mem_size = mem_stack.size();
    int freed = free_n(mem_size);
    if (freed != mem_size)
      return; //TODO: error handling
  }

  const Literal* MemStack::get_data(const string &var_name) {
    auto dat = mem_store[var_name];
    if (dat == NULL) {
      dat = nullptr;
      //TODO error handling
      throw "variable doesn't exist";
    }
    return dat;
  }

  bool MemStack::replace(const string &var_name, const Literal &value) {
    auto location = mem_store.find(var_name);
    //if we need to insert instead
    if (location == mem_store.end() || location->second == nullptr)
      return false;
    //if the stored is the same as the "replacement"
    if (&value == location->second)
      return true;
    delete location->second;
    location->second = value.copy();
    return true;
  }

  bool MemStack::insert(const string &var_name, const Literal &value) {
    auto location = mem_store.find(var_name);
    if (location != mem_store.end() && location->second != nullptr)
      return false;
    mem_store[var_name] = value.copy();
    mem_stack.push(var_name);
    return true;
  }

  int MemStack::free_n(int n) {
    int freed = 0;
    while(freed < n) {
      if (mem_stack.size() == 0)
        break;
      auto last_var = mem_stack.top();
      delete mem_store[last_var];
      mem_store[last_var] = nullptr;
      mem_stack.pop();
      ++freed;
    }
    return freed;
  }