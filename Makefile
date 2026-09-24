# Basic Makefile for BioChroma project

# Define compilation variable and binary
CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Iinclude

# === Get OBJ, SRCS and dependencies  === 
SRCS_ROOT := $(wildcard *.cpp)
SRCS_SRC  := $(wildcard src/*.cpp)

OBJS := $(patsubst %.cpp, obj/%.o, $(SRCS_ROOT)) \
        $(patsubst src/%.cpp, obj/%.o, $(SRCS_SRC))

# Commands to ignore generated files
.PHONY: all clean run

# === DEFINE COMPILATION RULES ===

# Objects compilation 
obj/%.o: src/%.cpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ./main.cpp compilation 
obj/%.o: %.cpp 
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Linking 
bin/agar: $(OBJS)
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@

all: bin/agar

# === DEFINE OTHER COMMANDS ===

clean:
	rm -rf obj bin 

run: bin/agar
	./bin/agar








