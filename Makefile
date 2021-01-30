CXX = g++
CXXFLAGS += -Wall -Wextra -g
CPPFLAGS += -std=c++11

OBJ_DIR = obj
MKDIR_P = mkdir -p

MODULES = lexer AST memory

OBJECTS = $(MODULES:%=$(OBJ_DIR)/%.o)
GTEST_LIB = /usr/local/lib/gtest/libgtest.a /usr/local/lib/gtest/libgtest_main.a

MAIN = main

all: $(MAIN)

$(OBJ_DIR):
	$(MKDIR_P) $(OBJ_DIR)

$(OBJ_DIR)/%.o: src/%.cpp $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

$(OBJ_DIR)/tests.o: tst/tests.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

tests: $(OBJ_DIR)/tests.o $(OBJECTS) $(GTEST_LIB)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ 

$(MAIN): $(OBJ_DIR)/$(MAIN).o  $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $^ -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(MAIN)
	rm -f tests