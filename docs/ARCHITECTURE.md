# BioChroma Pipeline and Architecture Description

> This document describes the system architecture, class relationships, and data pipeline of the project.

## Table of Contents
- [BioChroma Pipeline and Architecture Description](#biochroma-pipeline-and-architecture-description)
  - [Table of Contents](#table-of-contents)
  - [1. Project Overview](#1-project-overview)
  - [2. Architecture \& Class Diagrams](#2-architecture--class-diagrams)
    - [2.1 Data Structures](#21-data-structures)
    - [2.2 Class Diagram](#22-class-diagram)
  - [3. Data Pipeline](#3-data-pipeline)

---

## 1. Project Overview

This project is developed as an exploratory C++ portfolio project focused on scientific computing and software design. The primary goal is to implement complex Markov chain processes for multiple bacteria simulation in a virtual Petri dish.

Bacterial agents move, consume resources, undergo cell division, and interact locally. Their individual and collective evolution depends on:
- Nutrient availability across the continuous domain.
- Local population density (competition for resources and physical exclusion).
- Stochastic parameter mutations occurring during cell division.

Each bacterium is governed by an independent Markov process controlling state transitions (active, dividing, spore, or dead) and behavioral parameters (velocity, metabolic rate, chemotactic bias).

---

## 2. Architecture & Class Diagrams

Here are presented all the structures and classes used in the project.

### 2.1 Data Structures

**Cell States.** First, we need to define some basic structures. `BacteriumState` defines the current state of a `Bacterium` object: `DIVISION` means that the cell is dividing itself into two parts to produce two clones, and `SPORE` is an available state for `StandardBacterium` objects reached when nearby resources are too low.

**The Grid.** The Petri dish is modeled by an $N \times N$ discrete grid stored in a dynamically allocated `float` array (`nutrients`). This grid represents the local concentration of nutrients (normalized values between `0.0` and `9.0`), where `-1.0` encodes impenetrable obstacles (`#`). It is initialized directly by the `Simulation` parser from an ASCII configuration file.

> **NOTE**: Bacterial agents are not stored inside the grid cells. The `Simulation` orchestrator manages them on the heap through a dynamic pointer array (`Bacterium** bacteria`) using a Swap & Pop pattern for $\mathcal{O}(1)$ deletions. Continuous spatial coordinates ($\mathbb{R}^2$) are tracked via `Vector2`.

```cpp
// Internal states of a bacterium
enum class BacteriumState : uint8_t {
    ALIVE,      // Active agent: moves, feeds, metabolizes
    DEAD,       // Inactive agent scheduled for cleanup
    DIVISION,   // Undergoing binary fission
    SPORE       // Dormant state under severe nutrient scarcity
};
```

**Modular Subsystems.** The code base is organized into decoupled domain modules:
- `core/`: Fundamental math structures (`Vector2`), data containers (`Queue<T>`), and common types (`Types.h`).
- `entities/`: Autonomous biological agents (`Bacterium`, `StandardBacterium`, `ChemotacticBacterium`).
- `simulation/`: Environment simulation (`Grid`), run configurations (`SimulationConfig`), and engine orchestrator (`Simulation`).
- `io/`: CLI argument parsing (`CmdParser`) and telemetry logging (`Logger`).

**Generic FIFO Queue.** A standalone dynamic singly linked list `Queue<T>` (defined in `include/core/Queue.h`) handles sequential event pipelines, generation tracking, and telemetry buffers with strict $\mathcal{O}(1)$ push/pop guarantees.

### 2.2 Class Diagram

**Simulation Objects.** Below are represented all the simulation classes and their inheritance and usage relationships. The main class is `Simulation`, which orchestrates the simulation steps, holding the environment `PetriDish` and managing all polymorphic `Bacterium` instances.

**Bacterium Types.** Two concrete `Bacterium` child classes are implemented:
- `StandardBacterium`: A basic bacterium walking randomly on the continuous domain, capable of entering a `SPORE` dormant state when local nutrient concentrations drop below a critical threshold.
- `ChemotacticBacterium`: Cannot enter spore state, but moves along the local nutrient gradient (chemotaxis) with higher motility.

```mermaid 
classDiagram
    namespace core {
        class BacteriumState {
            <<enumeration>>
            ALIVE
            DEAD
            DIVISION
            SPORE
        }

        class Vector2 {
            +float x
            +float y
            +Vector2()
            +Vector2(float x, float y)
            +operator+(const Vector2& other) const Vector2
            +operator*(float scalar) const Vector2
        }

        class Queue~T~ {
            -Node* head
            -Node* tail
            -size_t size_
            +Queue()
            +~Queue()
            +push(const T& value) void
            +pop() void
            +front() T&
            +back() T&
            +empty() const bool
            +size() const size_t
            +clear() void
        }
    }

    namespace entities {
        class Bacterium {
            <<abstract>>
            #Vector2 position
            #BacteriumState state
            #float energy
            #float velocity
            #float mutation_prob
            #uint32_t id
            +virtual update(float dt, float local_nutrient)* void
            +virtual clone()* Bacterium*
            +get_id() const uint32_t
            +set_id(uint32_t id) void
        }

        class StandardBacterium {
            +update(float dt, float local_nutrient) override void
            +clone() override Bacterium*
        }

        class ChemotacticBacterium {
            -float sensing_radius
            +update(float dt, float local_nutrient) override void
            +clone() override Bacterium*
        }
    }

    namespace simulation {
        class SimulationConfig {
            +string grid_filepath
            +int total_steps
            +float dt
            +bool headless
            +bool real_time
            +float display_delay
            +uint32_t seed
            +string export_csv
            +SimulationConfig()
            +SimulationConfig(string, int, float, bool, bool, float, uint32_t, string)
        }

        class Grid {
            -int N
            -float cell_size
            -float* nutrients
            +Grid(int n, float cell_sz)
            +~Grid()
            +set_nutrient(int x, int y, float value) void
            +put_obstacle(int x, int y) void
            +is_obstacle(int x, int y) const bool
            +diffuse(float dt, float diffusion_coeff) void
            +get_nutrient_at(Vector2 pos) const float
            +consume_nutrient_at(Vector2 pos, float amount) void
            +get_global_energy() const float
            +get_n() const int
        }

        class Simulation {
            -Bacterium** bacteria
            -int n_bacteria
            -int max_bacteria
            -uint32_t next_bacterium_id
            -Grid* grid
            -SimulationConfig config
            -uint32_t current_step
            +Simulation(const SimulationConfig& config)
            +Simulation(const string& filepath)
            +~Simulation()
            +step() void
            +launch() void
            +display() void
            +wrap_position(const Vector2& pos, int N) Vector2
        }
    }

    Bacterium <|-- StandardBacterium
    Bacterium <|-- ChemotacticBacterium
    Bacterium *-- Vector2
    Bacterium *-- BacteriumState
    Simulation *-- Grid : owns
    Simulation *-- SimulationConfig : holds
    Simulation o-- Bacterium : manages (Bacterium**)
```

---

## 3. Data Pipeline

The simulation runs through a discrete-time execution pipeline:
1. **Initialization**: The configuration file is parsed by `Simulation(filepath)`, allocating the internal `Grid*` buffer and initial `Bacterium*` heap pointers.
2. **Temporal Step** (`Simulation::step()`):
   - **2.1 Diffusion**: Nutrient diffusion runs across the grid via explicit finite differences (`grid->diffuse(dt, diffusion_coeff)`), preserving obstacle boundaries.
   - **2.2 Nutrient Intake & Metabolic Update**: Each active bacterium consumes available local nutrients (`grid->consume_nutrient_at`) and executes `b->update(dt, nut)`.
   - **2.3 Fission**: Cells flagged with `BacteriumState::DIVISION` spawn child instances via `b->clone()`, receiving a unique `next_bacterium_id`.
   - **2.4 Population Pruning**: Inactive agents flagged as `DEAD` are freed with `delete` and removed in $\mathcal{O}(1)$ using the Swap & Pop method.
3. **Visualization & Telemetry**: `Simulation::display()` prints ANSI-colored terminal views of the grid, nutrient densities, obstacles, and active agents.

```mermaid 

flowchart TD
    Config[(CLI Flags / ASCII .txt)] -->|Init Params| Init[Simulation Initialization]

    subgraph EngineLoop [Temporal Simulation Loop: step dt]
        Init --> Diffuse[1. Nutrient Diffusion: 5-point stencil]
        Diffuse --> Markov[2. Random Walk & Markov Transitions]
        Markov --> Consume[3. Nutrient Uptake & Energy Consumption]
        Consume --> Events{Division or Death?}
        Events -->|Yes| UpdatePop[Vector Update: delete DEAD / clone instances]
        Events -->|No| CheckExport
        UpdatePop --> CheckExport{t % log_interval == 0 ?}
    end

    CheckExport -->|No| NextStep[t = t + dt]
    CheckExport -->|Yes| ExportBinary[(Binary Snapshots: pop, nutrients)]
    ExportBinary --> NextStep
    NextStep --> Diffuse

    ExportBinary -.->|Post-run conversion| ExportCSV[(CSV Datasets)]
    ExportCSV -.->|Python Analysis| PyPlots[Plots & Metrics]
```