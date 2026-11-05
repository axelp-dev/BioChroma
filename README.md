# BioChroma 

A C++ Markov chain based Petri dish simulator for multiple bacteria species evolution. Uses a C++20 implementations and a `agar` CLI launcher. 

## 1. Overview

**BioChroma** simulates the competitive dynamics and evolutionary adaptation of bacterial colonies on a 2D continuous surface. 

- **Microscopic layer**: Each bacterium acts as an autonomous agent driven by an independent Markov process governing its metabolism, motility (biased random walk / chemotaxis), and lifecycle transitions.
- **Macroscopic environment**: A continuous-to-discrete 2D nutrient grid subjected to consumption and 5-point stencil finite-difference diffusion.
- **Data analysis**: Headless simulation metrics are exported as CSV files for post-processing and population dynamics plotting via Python/Matplotlib.

Detailed technical specifications and class diagrams are available in [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md).  
Step-by-step developer tasks and milestone checklists are tracked in [`docs/GUIDELINES.md`](docs/GUIDELINES.md).
Changes are tracked [`docs/GUIDELINES.md`](docs/GUIDELINES.md).


## 2. Prerequisites

- **C++ Compiler**: GCC (>= 11).
- **Build System**: Make.
<!-- - **Python (Optional, for plots)**: Python 3.9+ with `matplotlib` and `pandas`. -->
<!-- - **Graphics (Upcoming v0.2)**: [Raylib](https://www.raylib.com/) (`brew install raylib`). -->

## 3. Quick Start 

### 3.1 Clone repo and test it 

Start by cloning the repo into your current workspace : 
```bash 
git clone https://github.com/axelp-dev/BioChroma
```
and install prerequisites. Next, you can simply update the local `CXX` variable with your C++ compilator path. And then simply use the provided `Makefile` to compile and run the project: 
```bash 
make 
make run 
```
To launch unit tests you ca simply use the `Makefile` command: 
```bash 
make test 
```
and a colored CLI display will present the tests results. Finnally, run `make clean` to delete build files and folders. 

### 3.2 Usage

**CLI Wraper.** A complete CLI launched is provided by `main.cpp`, and the project executable can be found in `./bin/agar` after compilation. Below are presented the launcher arguments: 
- `-d`, `--duration`: Simulation duration in steps, 
- `-t`, `--dt`: Simulation step duration (dt) in seconds, 
- `-v`, `--verbose`: Display simulation rendering in CLI, 
- `-s`, `--seed`: Random seed for all random draws to ensure reproductibility (not implemented yet), 
- `-r`, `--real-time`: Erase previous display at each step, 
- `-e`, `--delay`: Delay between each displays, 
- `-g`, `--grid-filepath`: Initialization Grid filepath, 
- `-o`, `--export-csv`: Export CSV filepath for statistics, 

**Examples**: 
```bash 
./bin/agar -d 70 -v -r -g data/grid-examples/oasis_grid10.txt -o data/test_export.csv

./bin/agar -d 70  -r -g data/grid-examples/example_grid10.txt
```

**Grid Examples.** A simulation initial state is defined by an ASCII grid. You can define your one grid using the templates in `data/grid-examples/`. 


## Architecture Summary 

```text 
BioChroma/
├── bin/                    # Compiled binaries (agar, run_tests)
├── data/                   # Input scenarios and test configs
│   └── grid-examples/
├── docs/
│   ├── ARCHITECTURE.md     # System architecture, classes & data pipeline
│   ├── GUIDELINES.md       # Step-by-step engineering roadmap & task breakdown
│   ├── MATHEMATICS.md      # Differential equations & stochastic formalisms
│   ├── PRESENTATION.md     # Project overview & slide summaries
│   └── QUEUE.md            # Generic Queue data structure specification
├── include/                # Modular public headers
│   ├── core/               # Math algebra, types, and generic data structures
│   │   ├── Queue.h         # Generic template FIFO container
│   │   ├── Types.h         # Common enumerations (BacteriumState)
│   │   └── Vector2.h       # 2D continuous space vector algebra
│   ├── entities/           # Simulation autonomous agents
│   │   └── Bacterium.h     # Base & polymorphic bacterium agent interface
│   ├── io/                 # Input parsing & logging utilities
│   │   ├── CmdParser.h     # Header-only CLI argument parser
│   │   └── Logger.h        # Telemetry & metrics logging interface
│   ├── simulation/         # Core orchestrator & environmental layer
│   │   ├── Grid.h          # Discrete 2D nutrient/obstacle grid
│   │   ├── Simulation.h    # Engine step loop & ANSI terminal visualizer
│   │   └── SimulationConfig.h # Execution configuration encapsulation
│   └── stats/              # Telemetry & metrics collection
│       └── Stats.h         # Stats module & BacteriumRecord container
├── src/                    # Source files grouped by functional domain
│   ├── core/
│   │   ├── Queue.cpp       # Explicit instantiations / queue utilities
│   │   └── Vector2.cpp
│   ├── entities/
│   │   └── Bacterium.cpp
│   ├── simulation/
│   │   ├── Grid.cpp
│   │   └── Simulation.cpp
│   └── stats/
│       └── Stats.cpp       # Telemetry logic and CSV exports
├── tests/                  # Unit test suites & assertions runner
│   ├── main_test.cpp       # Main test harness runner
│   ├── test_bacterium.cpp
│   ├── test_grid.cpp
│   ├── test_queue.cpp
│   ├── test_simulation.cpp
│   ├── test_simulation_config.cpp
│   ├── test_stats.cpp      # Stats & BacteriumRecord logic
│   ├── test_vector2.cpp
│   └── TestRunner.h
├── main.cpp                # CLI entry point
├── Makefile                # Build system (app, tests, sanitizer)
├── CHANGELOG.md
└── README.md
```