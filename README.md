# BioChroma 

A C++ Markov chain based Petri dish simulator for multiple bacteria species evolution. Uses a C++20 implementations and a `kallisto` CLI launcher (unreleased). 

## Overview

**BioChroma** simulates the competitive dynamics and evolutionary adaptation of bacterial colonies on a 2D continuous surface. 

- **Microscopic layer**: Each bacterium acts as an autonomous agent driven by an independent Markov process governing its metabolism, motility (biased random walk / chemotaxis), and lifecycle transitions.
- **Macroscopic environment**: A continuous-to-discrete 2D nutrient grid subjected to consumption and 5-point stencil finite-difference diffusion.
- **Data analysis**: Headless simulation metrics are exported as CSV files for post-processing and population dynamics plotting via Python/Matplotlib.

Detailed technical specifications and class diagrams are available in [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md).  
Step-by-step developer tasks and milestone checklists are tracked in [`docs/GUIDELINES.md`](docs/GUIDELINES.md).


## Prerequisites

- **C++ Compiler**: Clang (LLVM >= 14) or GCC (>= 11) with full C++20 support.
- **Build System**: Make.
- **Python (Optional, for plots)**: Python 3.9+ with `matplotlib` and `pandas`.
- **Graphics (Upcoming v0.2)**: [Raylib](https://www.raylib.com/) (`brew install raylib`).

## Quick Start 

Start by cloning the repo into your current workspace : 
```bash 
git clone https://github.com/axelp-dev/BioChroma
```
and install prerequisites. Next, you can simply update the local `CXX` variable with your C++ compilator path. And then simply use the provided `Makefile` to compile and run the project: 
```bash 
make 
make run 
make clean
```


## Architecture Summary 

```text 
BioChroma/
├── CMakeLists.txt        # Build definitions
├── Makefile              # Developer shortcuts
├── docs/                 # Architectural specifications & Mermaid diagrams
│   ├── ARCHITECTURE.md   # System architecture, classes & data pipeline
│   └── GUIDELINES.md     # Step-by-step engineering roadmap & task breakdown
├── include/              # Public headers (Types, Bacterium, PetriDish, Simulation)
├── src/                  # Source implementations and CLI entry point
├── tests/                # Unit test suites
├── scripts/              # Python plotting and validation scripts
└── data/                 # Output CSV logs
```