# BioChroma 

A C++ Markov chain based Petri dish simulator for multiple bacteria species evolution. Uses a C++20 implementations and a `kallisto` CLI launcher (unreleased). 
## Overview

**BioChroma** simulates the competitive dynamics and evolutionary adaptation of bacterial colonies on a 2D continuous surface. 

- **Microscopic layer**: Each bacterium acts as an autonomous agent driven by an independent Markov process governing its metabolism, motility (biased random walk / chemotaxis), and lifecycle transitions.
- **Macroscopic environment**: A continuous-to-discrete 2D nutrient grid subjected to consumption and 5-point stencil finite-difference diffusion.
- **Data analysis**: Headless simulation metrics are exported as CSV files for post-processing and population dynamics plotting via Python/Matplotlib.

Detailed technical specifications and class diagrams are available in [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md).

## Project Status & Roadmap

- [ ] **v0.1 — Headless Core Engine**: Markov lifecycle, 2D diffusion, CSV telemetry.
- [ ] **v0.2 — Interactive Viewport**: Real-time rendering via Raylib.
- [ ] **v0.3 — Collective Dynamics**: Quorum sensing, crowding effects, and binary fission.
- [ ] **v0.4 — Evolutionary Drift**: Stochastic parameter mutations and phylogenetic tracking.

## Prerequisites

- **C++ Compiler**: Clang (LLVM >= 14) or GCC (>= 11) with full C++20 support.
- **Build System**: CMake (>= 3.20) and Make.
- **Python (Optional, for plots)**: Python 3.9+ with `matplotlib` and `pandas`.
- **Graphics (Upcoming v0.2)**: [Raylib](https://www.raylib.com/) (`brew install raylib`).

## Quick Start 

> NOTE : Not implemented yet

## Architecture Summary 

```text 
BioChroma/
├── CMakeLists.txt        # Build definitions
├── Makefile              # Developer shortcuts
├── docs/                 # Architectural specifications & Mermaid diagrams
├── include/              # Public headers (Types, Bacterium, PetriDish, Simulation)
├── src/                  # Source implementations and CLI entry point
├── tests/                # Unit test suites
├── scripts/              # Python plotting and validation scripts
└── data/                 # Output CSV logs
```