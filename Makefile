# Basic Makefile for BioChroma project

CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Iinclude

# === Get OBJ, SRCS and dependencies === 
SRCS_ROOT := $(wildcard *.cpp)
SRCS_SRC  := $(wildcard src/*.cpp)
TEST_SRCS := tests/main_test.cpp tests/test_vector2.cpp src/Vector2.cpp

OBJS := $(patsubst %.cpp, obj/%.o, $(SRCS_ROOT)) \
        $(patsubst src/%.cpp, obj/%.o, $(SRCS_SRC))

.PHONY: all clean run test

# Cible par défaut
all: bin/agar

# === DEFINE COMPILATION RULES ===

# Compile sources from src/
obj/%.o: src/%.cpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile root sources (ex. main.cpp)
obj/%.o: %.cpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Linking main app
bin/agar: $(OBJS)
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile and link all tests
bin/run_tests: $(TEST_SRCS)
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -Itests $^ -o $@

# === DEFINE OTHER COMMANDS ===

clean:
	rm -rf obj bin

run: bin/agar
	./bin/agar

test: bin/run_tests
	./bin/run_tests