# Basic Makefile for BioChroma project

CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Iinclude -Iinclude/core -Iinclude/entities -Iinclude/simulation -Iinclude/io -fsanitize=address -g -Wno-unknown-pragmas

# === Sources & Objects ===
SRCS      := main.cpp $(shell find src -name '*.cpp')
OBJS      := $(patsubst %.cpp, obj/%.o, $(SRCS))

TEST_SRCS := tests/main_test.cpp \
             tests/test_vector2.cpp src/core/Vector2.cpp \
             tests/test_grid.cpp src/simulation/Grid.cpp \
             tests/test_bacterium.cpp src/entities/Bacterium.cpp \
             tests/test_simulation.cpp src/simulation/Simulation.cpp \
             tests/test_simulation_config.cpp \
			 tests/test_queue.cpp 

.PHONY: all clean run test

# Cible par défaut
all: bin/agar

# === Compilation rule for any .cpp (root or subfolders) ===
obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Linking main app
bin/agar: $(OBJS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile and link all tests
bin/run_tests: $(TEST_SRCS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -Itests $^ -o $@

# === Utility commands ===
clean:
	rm -rf obj bin

run: bin/agar
	./bin/agar

test: bin/run_tests
	./bin/run_tests