CXX = g++
CXXFLAGS += -Wall -Wextra -g
CPPFLAGS += -isystem src -std=c++11

OBJ_DIR = obj
MKDIR_P = mkdir -p

MODULES = parser

OBJECTS = $(MODULES:%=$(OBJ_DIR)/%.o)
GTEST_LIB = /usr/local/lib/gtest/libgtest.a /usr/local/lib/gtest/libgtest_main.a

PARSER = parser

all: $(PARSER) tests

$(OBJ_DIR):
	$(MKDIR_P) $(OBJ_DIR)

$(OBJ_DIR)/%.o: src/%.cpp $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

$(OBJ_DIR)/tests.o: tst/tests.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

tests: $(OBJ_DIR)/tests.o $(OBJECTS) $(GTEST_LIB)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ 

$(PARSER): $(OBJ_DIR)/$(PARSER).o  $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $^ -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(PARSER)
	rm -f tests